/*
 * A229391  Number of nX7 0..3 arrays of the sum of the corresponding element,
 *          the element to the east and the element to the south in a larger
 *          (n+1)X8 0..1 array.
 * https://oeis.org/A229391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A229391 (n > 9):
   a(n) = 385*a(n-1) - 33152*a(n-2) + 33024*a(n-3)
          - 98560*a(n-4) + 8486912*a(n-5) - 8388608*a(n-6) */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len  = 0;

    /* initialise cache with the first 9 terms */
    if (!cache) {
        cache_cap = 10;                   /* 0..9 indices */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = 0;
        cache[1] = 6528U;
        cache[2] = 4346752U;
        cache[3] = 1353901580U;
        cache[4] = 358453456908ULL;
        cache[5] = seq_from_str("93250181644300");
        cache[6] = seq_from_str("24062262212427788");
        cache[7] = seq_from_str("6184286713271025676");
        cache[8] = seq_from_str("1586293889549249544204");
        cache[9] = seq_from_str("406490146565253022875660");
        cache_len = 9;
    }

    if (n <= cache_len) return cache[n];

    /* extend cache if necessary */
    size_t needed = (size_t)n + 1;   /* include index 0 */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms from cache_len+1 up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t t1 = (seq_t)385   * cache[i-1];
        seq_t t2 = (seq_t)33024  * cache[i-3];
        seq_t t3 = (seq_t)8486912 * cache[i-5];
        seq_t t4 = (seq_t)33152  * cache[i-2];
        seq_t t5 = (seq_t)98560  * cache[i-4];
        seq_t t6 = (seq_t)8388608 * cache[i-6];
        seq_t val = t1 + t2 + t3 - (t4 + t5 + t6);
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
