/*
 * A234561  Number of (n+1) X (5+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A234561: Number of (n+1) × (5+1) arrays over {0,1,2,3,4}
   with every 2 × 2 subblock having its diagonal sum differing from
   its antidiagonal sum by 4.
   Recurrence (valid for n > 14):

        a(n) =  4·a(n‑1) + 30·a(n‑2) – 132·a(n‑3) – 331·a(n‑4)
              + 1720·a(n‑5) + 1530·a(n‑6) – 11280·a(n‑7) – 1324·a(n‑8)
              + 39136·a(n‑9) – 12360·a(n‑10) – 67968·a(n‑11)
              + 39456·a(n‑12) + 46080·a(n‑13) – 34560·a(n‑14)

   Initial values (n = 1 … 14):

        a(1)=5506,  a(2)=8416,  a(3)=12750,  a(4)=23830,
        a(5)=42982, a(6)=94114, a(7)=195558, a(8)=479914,
        a(9)=1108726, a(10)=2964106, a(11)=7385190,
        a(12)=21074650, a(13)=55254742, a(14)=165527434
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache for computed values, 1‑indexed; cache[0] is unused */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of allocated entries (>=15 after init) */
static bool init_done = false;

/* initialise cache with the first 14 terms */
static void init_seeds(void)
{
    cache = (seq_t *)calloc(15, sizeof(seq_t));  /* indices 0..14 */
    if (!cache) abort();

    cache[1]  = 5506;
    cache[2]  = 8416;
    cache[3]  = 12750;
    cache[4]  = 23830;
    cache[5]  = 42982;
    cache[6]  = 94114;
    cache[7]  = 195558;
    cache[8]  = 479914;
    cache[9]  = 1108726;
    cache[10] = 2964106;
    cache[11] = 7385190;
    cache[12] = 21074650;
    cache[13] = 55254742;
    cache[14] = 165527434;

    cache_len = 15;   /* cache[1] .. cache[14] are valid */
    init_done = true;
}

/* compute a(n) for n >= 1, using memoisation */
seq_t a(idx_t n)
{
    if (!init_done) init_seeds();

    /* trivial bounds check */
    if (n == 0) return (seq_t)0;  /* undefined, but safe */
    if (n >= SIZE_MAX) abort();

    if ((size_t)n < cache_len) return cache[n];

    /* extend the cache up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* compute the recurrence; all coefficients fit in int32 */
        seq_t pos = (seq_t)4   * cache[i-1] +
                    (seq_t)30  * cache[i-2] +
                    (seq_t)1720 * cache[i-5] +
                    (seq_t)1530 * cache[i-6] +
                    (seq_t)39136 * cache[i-9] +
                    (seq_t)39456 * cache[i-12] +
                    (seq_t)46080 * cache[i-13];

        seq_t neg = (seq_t)132   * cache[i-3] +
                    (seq_t)331   * cache[i-4] +
                    (seq_t)11280 * cache[i-7] +
                    (seq_t)1324  * cache[i-8] +
                    (seq_t)12360 * cache[i-10] +
                    (seq_t)67968 * cache[i-11] +
                    (seq_t)34560 * cache[i-14];

        cache[i] = pos - neg;
    }

    cache_len = new_len;
    return cache[n];
}
