/*
 * A229166  Number of ordered ways to write n = x*(x+1)/2 + y with y*(y+1)/2 +
 *          1 prime, where x and y are nonnegative integers.
 * https://oeis.org/A229166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (x,y) ≥ 0 with * n = x*(x+1)/2 +
 *            y and y*(y+1)/2 + 1 prime. * * The implementation loops over x,
 *            computes the corresponding * y = n − Tₓ, and tests whether Tᵧ +
 *            1 is prime. * It uses a small sieve for trial‑division up to
 *            10⁵, * which is sufficient for n up to about...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* -------------------- small prime sieve -------------------- */

#define SIEVE_LIMIT 100000            /* enough for 64‑bit trial division */

static bool sieve_composite[SIEVE_LIMIT + 1];
static uint32_t primes_list[SIEVE_LIMIT / 10 + 1];
static int prime_count = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;

    for (int i = 0; i <= SIEVE_LIMIT; ++i) sieve_composite[i] = false;
    for (int i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve_composite[i]) {
            for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve_composite[j] = true;
        }
    }
    for (int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve_composite[i]) {
            primes_list[prime_count++] = (uint32_t)i;
        }
    }
    primes_ready = true;
}

/* 64‑bit deterministic trial division */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2)            return false;
    if (v % 2 == 0)       return v == 2;
    init_primes();
    for (int i = 0; i < prime_count; ++i) {
        uint32_t p = primes_list[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > v) break;
        if (v % p == 0) return false;
    }
    return true;
}

/* -------------------- sequence function -------------------- */

/* External linkage as required by the harness */
seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t x = 0; ; ++x) {
        /* Compute Tₓ = x*(x+1)/2 using 128‑bit to avoid overflow */
        __int128 tri128 = (__int128)x * (x + 1) / 2;
        if (tri128 > n) break;

        idx_t y = n - (idx_t)tri128;          /* y ≥ 0 by construction */

        /* Compute Tᵧ + 1 (fits in 64‑bit for all n up to about 10⁴) */
        uint64_t t = (uint64_t)y * ((uint64_t)y + 1) / 2 + 1;

        if (is_prime_uint64(t))
            ++count;                          /* count holds the result value */
    }

    return count;
}
