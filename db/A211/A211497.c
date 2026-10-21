/*
 * A211497  Number of (n+1)X(n+1) -4..4 symmetric matrices with every 2X2
 *          subblock having sum zero and two, three or four distinct values.
 * https://oeis.org/A211497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A211497: Number of (n+1)×(n+1)‑4…4 symmetric matrices
   with every 2×2 subblock having sum zero and two, three or four distinct
   values.

   Recurrence (empirical):
      a(n) = 21*a(n-1)   - 177*a(n-2) + 742*a(n-3) - 1477*a(n-4)
             + 665*a(n-5) + 1725*a(n-6) - 952*a(n-7)
             -1462*a(n-8) - 476*a(n-9) - 48*a(n-10)

   Initial values:
      a(1)=40, a(2)=180, a(3)=808, a(4)=3636, a(5)=16396,
      a(6)=74104, a(7)=335660, a(8)=1523420, a(9)=6925768,
      a(10)=31527492
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static 1‑based cache for computed terms */
    static seq_t *cache    = NULL;
    static size_t  cap     = 0;
    static idx_t   max_n   = 0;

    /* initialise cache with the first 10 terms once */
    if (cache == NULL) {
        cap = 11;                             /* indices 0..10 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        /* dummy at index 0 */
        cache[0] = 0;
        cache[1]  = 40ULL;
        cache[2]  = 180ULL;
        cache[3]  = 808ULL;
        cache[4]  = 3636ULL;
        cache[5]  = 16396ULL;
        cache[6]  = 74104ULL;
        cache[7]  = 335660ULL;
        cache[8]  = 1523420ULL;
        cache[9]  = 6925768ULL;
        cache[10] = 31527492ULL;
        max_n = 10;
    }

    /* extend cache if necessary */
    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        memcpy(new_cache, cache, (max_n + 1) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing terms using the recurrence */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t pos =  21ULL * cache[i-1]
                       + 742ULL * cache[i-3]
                       + 665ULL * cache[i-5]
                       +1725ULL * cache[i-6];
            seq_t neg = 177ULL * cache[i-2]
                       +1477ULL * cache[i-4]
                       + 952ULL * cache[i-7]
                       +1462ULL * cache[i-8]
                       + 476ULL * cache[i-9]
                       +  48ULL * cache[i-10];
            cache[i] = pos - neg;
        }
        max_n = n;
    }

    return cache[n];
}
