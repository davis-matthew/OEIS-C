/*
 * A069417  Number of 3 X n binary arrays with a path of adjacent 1's and no
 *          path of adjacent 0's from top row to bottom row.
 * https://oeis.org/A069417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A069417: Number of 3×n binary arrays with a path of
   adjacent 1's and no path of adjacent 0's from top row to bottom row.

   The sequence satisfies the linear recurrence
      a(n) = 13*a(n‑1) – 48*a(n‑2) + 40*a(n‑3) – 8*a(n‑4)   for n > 4
   with initial values
      a(1)=1, a(2)=15, a(3)=147, a(4)=1231.
   The implementation avoids signed subtraction by writing the
   recurrence as
      a(n) = (13*a(n‑1)+40*a(n‑3)) – (48*a(n‑2)+8*a(n‑4))
   which has a strictly non‑negative subtrahend for all n≥5.
*/

#include <stdlib.h>
#include "../utils/mul_small.h"   /* helper: seq_t mul_small(seq_t, unsigned int) */

static seq_t *cache = NULL;          /* array a[0..]  (a[0] unused) */
static size_t cache_size = 0;        /* number of entries currently stored */

/* compute a(n), n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; value for n=0 not required */

    /* Grow the cache up to index n (inclusive) */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;  /* need indices 0..n */

        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        /* initialise the part of the array that hasn't been computed yet */
        size_t i;
        for (i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;            /* unused (a[0]) */
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)15;
            } else if (i == 3) {
                cache[i] = (seq_t)147;
            } else if (i == 4) {
                cache[i] = (seq_t)1231;
            } else {
                /* recurrence: a(n) = (13*a(n‑1)+40*a(n‑3)) – (48*a(n‑2)+8*a(n‑4)) */
                seq_t sum1 = mul_small(cache[i-1], 13) + mul_small(cache[i-3], 40);
                seq_t sum2 = mul_small(cache[i-2], 48) + mul_small(cache[i-4], 8);
                cache[i] = sum1 - sum2;
            }
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
}
