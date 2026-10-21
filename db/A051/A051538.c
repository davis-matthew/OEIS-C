/*
 * A051538  Least common multiple of {b(1),...,b(n)}, where b(k) =
 *          k(k+1)(2k+1)/6 = A000330(k).
 * https://oeis.org/A051538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = lcm( b(1), …, b(n) ) where b(k) = k(k+1)(2k+1)/6 =
 *            A000330(k) (sum of squares up to k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   calc_b
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static cache of already computed values.
 * It uses a simple Euclidean algorithm for gcd and performs the
 * lcm incrementally so that calls with arbitrary n are O(n) in the
 * worst case, and O(1) afterwards. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/calc_b.h"

/* Euclidean GCD for the type seq_t (an unsigned integer type). */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Return the n‑th term of A051538. */
seq_t a(idx_t n)
{
    if (n == 0u)          /* values below offset are irrelevant */
        return (seq_t)0;

    static seq_t *cache = NULL;        /* cache[i-1] = a(i) */
    static idx_t cache_upto = 0;       /* highest index computed */
    static size_t cache_size = 0;      /* allocated array size */

    /* Ensure the cache can hold n elements. */
    if (cache == NULL) {
        cache_size = (size_t)n;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    } else if (cache_size < (size_t)n) {
        cache_size = (size_t)n;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* Compute terms up to n if needed. */
    if (cache_upto < n) {
        /* Start from the previously computed lcm. */
        seq_t lcm_val = (cache_upto > 0u) ? cache[cache_upto - 1] : (seq_t)1;

        for (idx_t k = cache_upto + 1; k <= n; ++k) {
            seq_t bk = calc_b(k);
            seq_t g  = seq_gcd(lcm_val, bk);
            lcm_val = (lcm_val / g) * bk;      /* lcm(lcm_val, bk) */
            cache[k - 1] = lcm_val;
        }
        cache_upto = n;
    }

    return cache[n - 1];
}
