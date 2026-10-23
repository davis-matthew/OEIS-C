/*
 * A151951  a(1) = 1113; thereafter a(n) = (a(n-1) with digits sorted into
 *          descending order) - (a(n-1) with digits sorted into ascending
 *          order) (see the Kaprekar map, A151949).
 * https://oeis.org/A151951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1113; thereafter a(n) = (a(n-1) with digits sorted
   in descending order) minus (a(n-1) with digits sorted in ascending order).
   This is the Kaprekar map for 4‑digit numbers, which for 1113 eventually
   reaches the fixed point 6174. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* out of range, but harmless      */

    /* static cache: cache[i] holds a(i). 
       cache[0] is unused. last_computed holds the highest index stored. */
    static seq_t *cache = NULL;
    static idx_t last_computed = 0;

    /* initialise cache with a(1) if necessary */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);    /* indices 0 and 1   */
        if (!cache) return (seq_t)0;                  /* allocation failed */
        cache[0] = (seq_t)0;                          /* unused            */
        cache[1] = (seq_t)1113;                       /* a(1)              */
        last_computed = 1;
    }

    /* extend cache up to requested index n */
    if (n > last_computed) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0;                    /* allocation failed */
        cache = tmp;

        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            unsigned char cnt[10] = {0};

            /* count the digits of prev */
            digit_counts(prev, cnt);

            /* build ascending number */
            seq_t asc = (seq_t)0;
            for (unsigned d = 0; d < 10; ++d) {
                for (unsigned k = 0; k < cnt[d]; ++k)
                    asc = asc * (seq_t)10 + (seq_t)d;
            }

            /* build descending number */
            seq_t desc = (seq_t)0;
            for (int d = 9; d >= 0; --d) {
                for (unsigned k = 0; k < cnt[d]; ++k)
                    desc = desc * (seq_t)10 + (seq_t)d;
            }

            cache[i] = desc - asc;
        }
        last_computed = n;
    }

    return cache[n];
}
