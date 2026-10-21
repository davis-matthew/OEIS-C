/*
 * A235431  The smallest positive number that must be added to or subtracted
 *          from the sum of the first n primes in order to get a prime.
 * https://oeis.org/A235431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, hear
 * Formula:   a(n) = the smallest positive integer k such that (sum of the
 *            first n primes) ± k is prime. Algorithm: compute the sum S of
 *            the first n primes, then increment k from 1 until either S-k or
 *            S+k is prime
 * Range:     u8 n <= 14, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
/* Minimal headers – the build supplies the primitive types          */
/* ------------------------------------------------------------------ */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"          /* optional – used if seq_t > 64‑bit */

/* ------------------------------------------------------------------ */
/* Prime generation – incremental trial division                      */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list   = NULL;  /* array of primes in 64‑bit form */
static size_t   prime_cnt    = 0;      /* number of primes stored     */
static size_t   prime_cap    = 0;      /* allocated capacity          */

/* Ensure that at least `need` primes are available.                */
static void
ensure_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need)                 /* already enough       */
        return;

    /* Grow the prime array if necessary.                         */
    if (prime_cap < (size_t)need) {
        size_t new_cap = prime_cap ? prime_cap : 16;
        while (new_cap < (size_t)need)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);                /* allocation failed   */
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    /* Start testing candidates just after the last stored prime.  */
    uint64_t candidate = prime_cnt ? prime_list[prime_cnt-1] + 1 : 2;

    while (prime_cnt < (size_t)need) {
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate)               /* no further divisor needed */
                break;
            if (candidate % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            prime_list[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
/* 64‑bit deterministic Miller–Rabin                                    */
/* ------------------------------------------------------------------ */
static inline uint64_t
mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static inline uint64_t
mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mod_mul_u64(result, base, mod);
        base = mod_mul_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool
is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,
                                     17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t p = small[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* Bases sufficient for deterministic 64‑bit MR. */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    uint64_t d = n-1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* Wrapper that works with both native and arbitrary‑size seq_t.      */
/* For values that do not fit in 64 bits we fall back to the big     */
/* primality routine provided by the build.                         */
static inline bool
is_prime_seq(seq_t val)
{
    if ((uint64_t)val < 2)
        return false;
    if (val <= (seq_t)(UINT64_MAX))
        return is_prime_u64((uint64_t)val);
    /* Arithmetic types larger than 64 bits are handled by the      */
    /* build‑specific implementation.                                 */
    return is_prime_big(val);
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1 – defensive */
        return 0;

    ensure_primes(n);

    /* Compute the sum S of the first n primes. */
    seq_t sum = 0;
    for (size_t i = 0; i < (size_t)n; ++i) {
        sum += (seq_t)prime_list[i];
    }

    /* Search for the smallest positive k. */
    for (seq_t k = 1; ; ++k) {
        if (sum >= k && is_prime_seq(sum - k))
            return k;
        if (is_prime_seq(sum + k))
            return k;
    }
}
