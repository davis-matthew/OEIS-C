/*
 * A092438  Sequence arising from enumeration of domino tilings of Aztec
 *          Pillow-like regions.
 * https://oeis.org/A092438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula (recurrence): a(n) = 5*a(n-1) - 5*a(n-2) - 5*a(n-3) + 6*a(n-4);
   initial terms: a(0)=0, a(1)=2, a(2)=6, a(3)=26
   (computed for any n ≥ 0, in any order of calls)
*/

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;   /* static memoisation table          */
static idx_t   cache_size = 0;/* number of entries currently cached */

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;      /* allocation failed – return dummy */
        cache = new_cache;

        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0)          cache[i] = (seq_t)0;
            else if (i == 1)     cache[i] = (seq_t)2;
            else if (i == 2)     cache[i] = (seq_t)6;
            else if (i == 3)     cache[i] = (seq_t)26;
            else {
                seq_t p1 = mul_small(cache[i-1], 5U);
                seq_t p2 = mul_small(cache[i-2], 5U);
                seq_t p3 = mul_small(cache[i-3], 5U);
                seq_t p4 = mul_small(cache[i-4], 6U);

                /* Compute a(i) = 5*a(i-1) - 5*a(i-2) - 5*a(i-3) + 6*a(i-4) */
                seq_t res = p1 + p4;   /* additions in unsigned type are well‑defined modulo 2^bits */
                res -= p2;
                res -= p3;
                cache[i] = res;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
