/*
 * A213420  Smallest number k such that the sum of prime factors of k (counted
 *          with multiplicity) is n times a square > 1.
 * https://oeis.org/A213420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 747, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A213420:  a(n) = smallest k such that sum_of_primitive_prime_factors(k)
 * (counted with multiplicity) = n * q²  for some integer q ≥ 2.
 *
 * The algorithm scans integers k in increasing order, computes the
 * summatory function sopfr(k) (sum of prime factors with multiplicity)
 * and records the first k that yields each value of n = sopfr(k)/q²
 * for any q ≥ 2.  A static cache holds the smallest k for each n that
 * has already been found; the function expands the cache lazily.
 * This implementation only uses the standard library and the
 * helper <stdint.h>, <stdlib.h> and <stdbool.h> – no external
 * utilities are required.  The helper function `sopfr_uint64`
 * performs factorisation by trial division, sufficient for the
 * required range of k (n up to ≈10000). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>   /* for bool */

/* --------------------------------------------------------------------- */
/* Sum of prime factors with multiplicity for a 64‑bit integer            */
/* --------------------------------------------------------------------- */
static seq_t sopfr_uint64(uint64_t n)
{
    if (n <= 1) return 0;

    seq_t sum = 0;
    uint64_t m = n;
    /* factor 2 */
    while ((m & 1) == 0) {
        sum += 2;
        m >>= 1;
    }
    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) {
            sum += p;
            m /= p;
        }
    }
    if (m > 1) sum += m;    /* remaining prime factor */
    return sum;
}

/* --------------------------------------------------------------------- */
/* Static cache of minimal k for each value of n (1‑based indexing)       */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;          /* cache[0] is unused */
static idx_t cache_size = 0;         /* number of allocated elements     */
static idx_t cur_k = 2;              /* next integer to examine          */

/* Ensure the cache can address index <= n; new slots are zeroed. */
static void ensure_cache_size(idx_t n)
{
    if (cache_size > n) return;
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);          /* allocation failed */
    /* zero new elements */
    for (idx_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_size = new_size;
}

/* Extend the scan until the cache entry for n is non‑zero. */
static void find_until(idx_t n)
{
    if (n == 0) return;                       /* ignore index 0 */
    ensure_cache_size(n);
    if (cache[n] != 0) return;                /* already known */

    /* Continue scanning while the required entry remains unset. */
    while (cache[n] == 0) {
        seq_t sum_val = sopfr_uint64(cur_k);
        uint64_t sum = (uint64_t)sum_val;     /* safe; sum fits in 64 bits for the range */

        /* For every possible q≥2 with q² dividing sum, update cache. */
        for (uint64_t q = 2; q * q <= sum; ++q) {
            uint64_t q2 = q * q;
            if (sum % q2 != 0) continue;
            uint64_t idx = sum / q2;
            if (idx == 0) continue;          /* skip n=0 */
            ensure_cache_size((idx_t)idx);
            if (cache[(idx_t)idx] == 0)
                cache[(idx_t)idx] = cur_k;
        }
        ++cur_k;
    }
}

/* --------------------------------------------------------------------- */
/* Public entry point:  compute a(n) for the sequence A213420.           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1 */
    find_until(n);
    return cache[n];
}
