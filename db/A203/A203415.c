/*
 * A203415  Vandermonde determinant of the first n nonprimes (A018252).
 * https://oeis.org/A203415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A203415(n) = Vandermonde determinant of the first n nonprimes *
 *            (nonprimes = 1, 4, 6, 8, 9, 10, 12, ...)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

/* global caches */
static size_t cache_cap = 0, cache_len = 0;
static seq_t *cache = NULL;
static size_t nonprime_cap = 0, nonprime_len = 0;
static uint64_t *nonprimes = NULL;
static idx_t next_candidate = 1;  /* last checked candidate */

/* ensure nonprimes array has at least 'needed' elements */
static void ensure_nonprime_cap(size_t needed)
{
    if (needed > nonprime_cap)
        ensure_capacity(needed, &nonprime_cap,
                        (void **)&nonprimes, sizeof(uint64_t));
}

/* ensure cache array has at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (needed > cache_cap)
        ensure_capacity(needed, &cache_cap,
                        (void **)&cache, sizeof(seq_t));
}

/* generate next nonprime number and append to nonprimes array */
static void append_next_nonprime(void)
{
    /* find next composite or 1 */
    do {
        ++next_candidate;
    } while (is_prime(next_candidate));

    ensure_nonprime_cap(nonprime_len + 1);
    nonprimes[nonprime_len++] = next_candidate;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of range, not used */

    /* initialise with first term if needed */
    if (cache_len == 0) {
        ensure_cache_cap(1);
        ensure_nonprime_cap(1);
        cache[0] = (seq_t)1;
        nonprimes[0] = (uint64_t)1;  /* first nonprime is 1 */
        nonprime_len = 1;
        cache_len = 1;
        next_candidate = 1;  /* next integer to test will be 2 */
    }

    /* grow up to requested n */
    while (cache_len < (size_t)n) {
        /* generate next nonprime number */
        append_next_nonprime();

        size_t k = cache_len;          /* new index (0-based) */
        idx_t x = (idx_t)nonprimes[k]; /* value of new nonprime */

        /* product of differences between new element and all previous */
        seq_t prod = (seq_t)1;
        for (size_t i = 0; i < k; ++i) {
            prod *= ((seq_t)(x - (idx_t)nonprimes[i]));
        }

        /* new Vandermonde determinant */
        ensure_cache_cap(k + 1);
        cache[k] = cache[k - 1] * prod;
        ++cache_len;
    }

    return cache[(size_t)n - 1];
}
