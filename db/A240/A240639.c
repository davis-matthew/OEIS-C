/*
 * A240639  Number of nX4 0..3 arrays with no element equal to one or three
 *          horizontal or vertical neighbors, with new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A240639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   seq_from_str, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence (OEIS A240639): 
   a(n) = 38*a(n-1) + 193*a(n-2) - 1416*a(n-3) - 7253*a(n-4)
          - 47234*a(n-5) - 212251*a(n-6) - 820896*a(n-7)
          - 1101555*a(n-8) + 3235842*a(n-9) + 4995027*a(n-10)
          + 822312*a(n-11) + 1108809*a(n-12) - 3188646*a(n-13)
          - 4782969*a(n-14)
   Initial values: a(1)=5, a(2)=193, a(3)=7993, a(4)=333308,
                   a(5)=13897353, a(6)=579476701, a(7)=24162243789,
                   a(8)=1007486056500, a(9)=42008844856285,
                   a(10)=1751630302786777, a(11)=73037207055319249,
                   a(12)=3045410673315147020, a(13)=126983581950165639585,
                   a(14)=5294796602205290417269
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* initialise cache with first 14 terms on first call */
    if (!cache) {
        cache_cap = 15;            /* indices 0..14 (0 unused) */
        cache = seq_realloc(NULL, 0, cache_cap);
        /* string representations of the first 14 terms (index 0 unused) */
        static const char *init_strs[15] = {
            "0",
            "5", "193", "7993", "333308", "13897353", "579476701",
            "24162243789", "1007486056500", "42008844856285",
            "1751630302786777", "73037207055319249",
            "3045410673315147020", "126983581950165639585",
            "5294796602205290417269"
        };
        for (size_t i = 1; i < cache_cap; ++i)
            cache[i] = seq_from_str(init_strs[i]);
    }

    /* ensure cache large enough for requested index */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        /* remember old capacity before resizing */
        size_t old_cap = cache_cap;
        seq_t *tmp = seq_realloc(cache, cache_cap, need);
        cache = tmp;
        cache_cap = need;
        /* compute missing terms from old_cap up to n */
        for (size_t i = old_cap; i <= (size_t)n; ++i) {
            /* i >= 15, we may skip the first 14 terms */
            if (i <= 14) continue;
            seq_t pos = 0, neg = 0;
            pos += 38 * cache[i-1];
            pos += 193 * cache[i-2];
            pos += 3235842 * cache[i-9];
            pos += 4995027 * cache[i-10];
            pos += 822312 * cache[i-11];
            pos += 1108809 * cache[i-12];

            neg += 1416 * cache[i-3];
            neg += 7253 * cache[i-4];
            neg += 47234 * cache[i-5];
            neg += 212251 * cache[i-6];
            neg += 820896 * cache[i-7];
            neg += 1101555 * cache[i-8];
            neg += 3188646 * cache[i-13];
            neg += 4782969 * cache[i-14];

            cache[i] = pos - neg;  /* pos >= neg for all valid n */
        }
    }

    return cache[n];
}
