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
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of A039340 by brute‑force search. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether x has the same number of non‑zero 1’s and 6’s in base 8. */
static bool has_equal_nonzero_ones_and_sixs(seq_t x)
{
    int c1 = 0, c6 = 0;
    while (x > 0) {
        seq_t d = x % (seq_t)8;          /* extract lowest base‑8 digit */
        if (d == (seq_t)1) ++c1;
        else if (d == (seq_t)6) ++c6;
        x = x / (seq_t)8;                /* move to next digit */
    }
    return (c1 > 0) && (c1 == c6);
}

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
                cache = mem_new(seq_t, 64);
                if (!cache) return 0;   /* allocation failure */
            } else if (cache_len % 64 == 0) {
                /* grow by 64 elements each time */
                seq_t *tmp = mem_resize(seq_t, cache, (cache_len + 64));
                if (!tmp) return 0;        /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = last_val;
        }
    }
    return cache[n - 1];
}
