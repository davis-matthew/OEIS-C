/*
 * A212680  Number of (w,x,y,z) with all terms in {1,...,n} and |x-y|=|y-z|+1.
 * https://oeis.org/A212680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + a(n-4) + 2*a(n-5) - a(n-6)
 *            Initial values: a(0)=0, a(1)=0, a(2)=4, a(3)=18, a(4)=56,
 *            a(5)=120 (Sequence A212680)
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure fallback */
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) {
            switch (i) {
                case 0:
                case 1:
                    cache[i] = 0;
                    break;
                case 2:
                    cache[i] = 4;
                    break;
                case 3:
                    cache[i] = 18;
                    break;
                case 4:
                    cache[i] = 56;
                    break;
                case 5:
                    cache[i] = 120;
                    break;
                default: {
                    /* compute using the 6‑term linear recurrence,
                       using only +–* operations with seq_t         */
                    seq_t term = cache[i-1] * 2;
                    term += cache[i-2];
                    term += cache[i-4];
                    term += cache[i-5] * 2;
                    term -= cache[i-3] * 4;
                    term -= cache[i-6];
                    cache[i] = term;
                }
            }
        }
        cache_cap = new_cap;
    }
    return cache[(size_t)n];
}
