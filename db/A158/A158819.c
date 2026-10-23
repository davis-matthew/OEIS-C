/*
 * A158819  a(n) = (number of squarefree numbers <= n) minus round(n/zeta(2)).
 * https://oeis.org/A158819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (# of squarefree integers ≤ n) – round(n / ζ(2))
 * Range:     i8 n <= 9621, i16 n <= 9621, i32 n <= 9621, i64 n <= 9621,
 *            i128 n <= 9621, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ζ(2) = π²/6 ≈ 1.6449340668482264365, so n / ζ(2) ≈ n * 0.6079271018540266 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Pre‑compute all primes up to 100 000.  For n ≤ 10 000 we only need
   primes up to √n ≤ 100, but a few more primes do not affect correctness
   and keep the routine simple.
   ------------------------------------------------------------------ */
static bool primes_initialized = false;
static size_t prime_cnt = 0;
#define MAX_PRIME_LIMIT 100000
static uint32_t prime_list[MAX_PRIME_LIMIT + 1];
static uint8_t sieve[MAX_PRIME_LIMIT + 1];

static void init_primes(void)
{
    if (primes_initialized) return;

    /* simple sieve of Eratosthenes */
    memset(sieve, 0, sizeof(sieve));
    for (size_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                sieve[j] = 1;

    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (!sieve[i]) prime_list[prime_cnt++] = (uint32_t)i;

    primes_initialized = true;
}

/* ------------------------------------------------------------------
   Return true iff x is squarefree: i.e. no prime square divides x.
   ------------------------------------------------------------------ */
static bool is_squarefree(uint64_t x)
{
    /* use the pre‑computed primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > x) break;
        if (x % pp == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------
   Main function required by the test harness.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure the prime list is ready */
    init_primes();

    /* Count squarefree numbers ≤ n */
    idx_t sf_count = 0;
    for (idx_t k = 1; k <= n; ++k)
        if (is_squarefree(k)) ++sf_count;

    /* Compute round(n / ζ(2)) using high‑resolution integer arithmetic */
    /* Rational approximation: ζ(2) ≈ π²/6 ≈ 1.6449340668482264365
       => 6/π² ≈ 0.6079271018540266 ≈ 6079271018540266 / 10¹⁶   */
    const __int128 SCALE = (__int128)10000000000000000ULL;      /* 10¹⁶ */
    const __int128 NUM   = (__int128)6079271018540266ULL;       /* 6/π² * SCALE rounded */
    /* tmp = n * NUM + SCALE/2  → proper rounding */
    __int128 tmp = (__int128)n * NUM + SCALE / 2;
    seq_t round_val = (seq_t)(tmp / SCALE);

    /* Result as signed integer */
    seq_t result = (seq_t)sf_count - round_val;
    return result;
}
