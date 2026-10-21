/*
 * A062032  Group odd numbers into (1), (3,5), (7,9,11), (13,15,17,19), ...;
 *          a(n) = product of n-th group.
 * https://oeis.org/A062032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n-1} (2*k + n*(n-1) + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build environment.  */
static seq_t *cache = NULL;          /* cached values a(1)..a(cached_n)   */
static idx_t cached_n = 0;          /* maximum index currently cached    */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; a(0) is undefined */
        return (seq_t)0;

    /* Allocate or extend the cache if necessary */
    if (cache == NULL || cached_n < n) {
        size_t new_size = (size_t)(n + 1);
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (cache == NULL) {
                /* allocation failure: return 0 as a safe default */
                return (seq_t)0;
            }
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (tmp == NULL) {
                /* reallocation failure: keep old cache */
                return (seq_t)0;
            }
            cache = tmp;
        }
        /* Compute values from cached_n+1 up to n */
        for (idx_t m = cached_n + 1; m <= n; ++m) {
            seq_t prod = (seq_t)1;
            seq_t start = (seq_t)m * (seq_t)(m - 1) + (seq_t)1;
            for (idx_t k = 0; k < m; ++k) {
                seq_t term = start + ((seq_t)2 * (seq_t)k);
                prod = prod * term;
            }
            cache[m] = prod;
        }
        cached_n = n;
    }
    return cache[n];
}
