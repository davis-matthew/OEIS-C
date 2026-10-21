/*
 * A228513  a(n) = Sum_{k=0..n} 2^k*(n!/k!)^2.
 * https://oeis.org/A228513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+2)*a(n-1) - 2*(n-1)^2*a(n-2) * with a(0) = 1, a(1) =
 *            3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Memoized cache for the sequence */
static seq_t *a_cache   = NULL;   /* array of computed terms */
static idx_t cache_size = 0;      /* allocated array size */
static idx_t cache_max  = 0;      /* largest n for which a_cache[n] is valid */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Allocate or enlarge the cache if necessary */
    if (a_cache == NULL) {
        cache_size = (n >= 1) ? n + 1 : 2;
        a_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!a_cache) return (seq_t)0;          /* allocation failed */
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)3;
        cache_max  = 1;
    } else if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Fallback: recompute the required value without caching. */
            seq_t prev2 = (seq_t)1;
            seq_t prev1 = (seq_t)3;
            seq_t cur   = prev1;
            for (idx_t k = 2; k <= n; ++k) {
                seq_t k_seq   = (seq_t)k;
                seq_t k_minus = (seq_t)(k - 1);
                cur = ((k_seq * k_seq) + (seq_t)2) * prev1
                    - ((seq_t)2 * k_minus * k_minus) * prev2;
                prev2 = prev1;
                prev1 = cur;
            }
            return cur;
        }
        a_cache   = tmp;
        cache_size = new_size;
        /* The existing cache contents are preserved by realloc. */
    }

    /* Compute any missing terms up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t k_seq   = (seq_t)k;
        seq_t k_minus = (seq_t)(k - 1);

        seq_t term1 = ((k_seq * k_seq) + (seq_t)2) * a_cache[k - 1];
        seq_t term2 = ((seq_t)2 * k_minus * k_minus) * a_cache[k - 2];
        a_cache[k] = term1 - term2;
    }
    cache_max = n;

    return a_cache[n];
}
