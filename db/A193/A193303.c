/*
 * A193303  Squarefree numbers multiplied by powers of three.
 * https://oeis.org/A193303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th integer of the form m·3^k with m squarefree (k ≥ 0),
 *            sorted in increasing order
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* In particular a(n) equals the n-th integer that has no repeated prime
   factors other than possibly 3.  The sequence is generated lazily: the
   array `cache` stores already discovered terms so that subsequent calls
   for any n are answered in O(1), while new terms are found by scanning
   the natural numbers one by one.  The test "squarefree after removing
   all 3‑factors" uses the utility function `is_squarefree`. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

static seq_t *cache     = NULL;   /* cached terms */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity of cache */
static idx_t  last_checked = 0;   /* largest integer examined so far */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* n=0 is outside the OEIS offset */

    /* Return from cache if available */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* Grow the cache until it contains at least n terms */
    while ((size_t)n > cache_len) {
        /* Next integer to examine */
        idx_t x = last_checked + 1;
        seq_t val = (seq_t)x;
        seq_t tmp = val;

        /* Strip off all powers of 3 */
        while (tmp % (seq_t)3 == (seq_t)0) {
            tmp /= (seq_t)3;
        }

        /* If the remaining part is squarefree, keep the whole number */
        if (is_squarefree(tmp)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *new_arr = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                if (!new_arr) { /* out of memory */
                    exit(EXIT_FAILURE);
                }
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }

        last_checked = x;
    }

    return cache[n - 1];
}
