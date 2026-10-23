/*
 * A044483  Numbers n such that string 0,0 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_double_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A044483.
   a(n) is the n‑th integer whose base‑6 representation contains the
   substring “00”, but the integer n+1 does not.
   The function searches integers in increasing order and memoises
   results in a static array, allowing re‑ordering of calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_double_zero.h"

/* Cache of previously found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {             /* undefined for n=0; return 0 as a guard */
        return 0;
    }

    /* Grow cache until it contains at least n elements */
    while (cache_len < (size_t)n) {
        seq_t candidate;
        if (cache_len == 0) {
            candidate = 1;   /* start searching from 1 */
        } else {
            candidate = cache[cache_len - 1] + 1;
        }

        /* Find the next qualifying number */
        for (;;) {
            if (has_double_zero(candidate) && !has_double_zero(candidate + 1)) {
                /* Ensure sufficient capacity */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {               /* allocation failed */
                        /* In a real library we'd handle this, but for
                           the purposes of the sequence generator we
                           simply abort execution. */
                        abort();
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
                break;                  /* move to next term */
            }
            candidate++;                /* try next integer */
        }
    }

    return cache[n - 1];
}
