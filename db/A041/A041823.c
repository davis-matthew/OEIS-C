/*
 * A041823  Denominators of continued fraction convergents to sqrt(432).
 * https://oeis.org/A041823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 47, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041823: Denominators of continued fraction convergents to sqrt(432)
   a(n) = 2702*a(n-8) - a(n-16)  for n > 15
   Initial values:
     a(0)=1  a(1)=1  a(2)=4  a(3)=5  a(4)=9  a(5)=14  a(6)=51  a(7)=65
     a(8)=2651 a(9)=2716 a(10)=10799 a(11)=13515 a(12)=24314 a(13)=37829
     a(14)=137801 a(15)=175630
*/

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache  = NULL;
    static size_t  cache_len = 0;

    /* initialise cache with the first 16 terms */
    if (cache == NULL) {
        cache_len = 16;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory, return 0 as a placeholder */

        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)4;
        cache[3]  = (seq_t)5;
        cache[4]  = (seq_t)9;
        cache[5]  = (seq_t)14;
        cache[6]  = (seq_t)51;
        cache[7]  = (seq_t)65;
        cache[8]  = (seq_t)2651;
        cache[9]  = (seq_t)2716;
        cache[10] = (seq_t)10799;
        cache[11] = (seq_t)13515;
        cache[12] = (seq_t)24314;
        cache[13] = (seq_t)37829;
        cache[14] = (seq_t)137801;
        cache[15] = (seq_t)175630;
    }

    /* if already computed, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow the cache to accommodate index n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) return 0; /* out of memory, return 0 as a placeholder */
    cache = tmp;
    cache_len = new_len;

    /* compute missing terms using the recurrence */
    for (size_t i = 16; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)2702 * cache[i-8] - cache[i-16];
    }

    return cache[(size_t)n];
}
