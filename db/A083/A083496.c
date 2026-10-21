/*
 * A083496  Multiples of 8 in which there is no common digit in successive
 *          terms.
 * https://oeis.org/A083496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   common_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A083496: Multiples of 8 in which there is no common digit in successive terms.
   This implementation generates the sequence lazily: it keeps an array of already
   computed values and extends it when a larger index is requested.  Each new term
   is the next multiple of 8 that does not share any decimal digit with the
   previously stored term.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/common_digit.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* last multiple index (k such that term = 8*k) and its value */
    static idx_t last_k = 1;
    static seq_t last_val = 0;

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();                /* allocation failure */
        cache[0] = (seq_t)8;                /* a(0) = 8 */
        cache_len = 1;
        last_k   = 1;                       /* 8*1 = 8 */
        last_val = cache[0];
    }

    /* extend cache until we have the requested index */
    while (cache_len <= n) {
        /* next multiple of 8 */
        ++last_k;
        seq_t cand = (seq_t)8 * (seq_t)last_k;

        /* keep only those that share no digit with the last term */
        if (!common_digit(last_val, cand)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                if (!cache) abort();        /* allocation failure */
            }
            cache[cache_len++] = cand;
            last_val = cand;
        }
    }

    return cache[n];
}
