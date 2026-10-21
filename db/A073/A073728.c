/*
 * A073728  a(n) = Sum_{k=0..n} S(k), where S(n) are the tribonacci
 *          generalized numbers A001644.
 * https://oeis.org/A073728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) a(0) = 3, a(1) = 4, a(2) = 7
 *            (tribonacci-like with shifted start)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>           /* size_t */

/* External integer types supplied by the build system. */
extern seq_t a(idx_t n);

/* Static memoisation tables.  The array is grown on demand. */
static seq_t *cache = NULL;
static idx_t   cache_size = 0;

/* Return the n‑th element of OEIS A073728.  */
seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)7;
    }

    /* Extend the cache up to the requested index. */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));

        for (idx_t i = old_size; i <= n; ++i) {
            /* The indices i-1, i-2, i-3 are safe because i ≥ old_size ≥ 3. */
            cache[(size_t)i] =
                cache[(size_t)(i-1)] + cache[(size_t)(i-2)] + cache[(size_t)(i-3)];
        }
    }

    return cache[(size_t)n];
}
