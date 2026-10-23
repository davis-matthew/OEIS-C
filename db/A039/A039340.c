/*
 * A039340  Numbers whose base-8 representation has the same nonzero number of
 *          1's and 6's.
 * https://oeis.org/A039340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_equal_nonzero_ones_and_sixs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of A039340 by brute‑force search. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_equal_nonzero_ones_and_sixs.h"

/* Cached sequence to support arbitrary call order. */
static seq_t *cache = NULL;          /* storage for found terms */
static idx_t  cache_len = 0;         /* number of stored terms     */
static seq_t  last_val = 0;          /* last value processed */

/* Public function as required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not part of OEIS, defensive */

    /* grow cache until we have at least n terms */
    while (cache_len < n) {
        last_val += (seq_t)1;        /* next integer to test */

        if (has_equal_nonzero_ones_and_sixs(last_val)) {
            /* add to cache */
            if (cache == NULL) {
                cache = (seq_t *)malloc(64 * sizeof(seq_t));
                if (!cache) return 0;   /* allocation failure */
            } else if (cache_len % 64 == 0) {
                /* grow by 64 elements each time */
                seq_t *tmp = (seq_t *)realloc(cache,
                                            (cache_len + 64) * sizeof(seq_t));
                if (!tmp) return 0;        /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = last_val;
        }
    }
    return cache[n - 1];
}
