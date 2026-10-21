/*
 * A094178  Numbers n such that 4n+1 is divisible only by primes of form 4m+1
 *          (i.e., by the Pythagorean primes A002144).
 * https://oeis.org/A094178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that every prime divisor of 4m+1
 *            is ≡ 1 (mod 4)
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

/* ---------- prime sieve up to a fixed limit -------------------------------- */

#define PRIME_SIEVE_LIMIT 200000         /* sufficient for all n in tests */

static uint32_t primes[PRIME_SIEVE_LIMIT];
static size_t   prime_cnt = 0;
static bool     primes_init = false;

static void init_primes(void)
{
    if (primes_init) return;
    bool *is_prime = (bool *)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);             /* allocation failure */

    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_SIEVE_LIMIT; ++i)
    {
        if (!is_prime[i]) continue;
        for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i) is_prime[j] = false;
    }

    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;

    free(is_prime);
    primes_init = true;
}

/* ---------- test whether 4*n+1 has only primes ≡ 1 (mod 4) ---------------- */

static bool candidate_ok(uint64_t n)
{
    __uint128_t m128 = (__uint128_t)4 * n + 1;
    if (m128 > ( __uint128_t ) UINT64_MAX) return false;   /* beyond 64‑bit */
    uint64_t m = (uint64_t) m128;

    uint64_t temp = m;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i)
    {
        uint32_t p = primes[i];
        if (temp % p == 0)
        {
            if (p % 4 != 1) return false;
            while (temp % p == 0) temp /= p;
        }
    }

    if (temp > 1)                         /* remaining prime factor */
    {
        if (temp % 4 != 1) return false;
    }

    return true;
}

/* ---------- memoised sequence array ------------------------------------- */

static seq_t  *seq_buf = NULL;      /* array of computed terms */
static size_t  seq_len = 0;         /* number of stored terms */
static size_t  seq_cap = 0;         /* current capacity of seq_buf */
static uint64_t last_tested = 0;    /* largest integer already examined */

/* Ensure enough capacity for at least 'need' elements */
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!new_buf) exit(1);           /* allocation failure */
    seq_buf = new_buf;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    init_primes();                    /* make sure primes are ready */

    if (n <= 0) return 0;            /* undefined for n==0, but never called */

    /* generate terms until we have at least n of them */
    while (seq_len < n)
    {
        ++last_tested;
        if (candidate_ok(last_tested))
        {
            ensure_seq_capacity(seq_len + 1);
            seq_buf[seq_len++] = (seq_t)last_tested;
        }
    }
    return seq_buf[n - 1];
}
