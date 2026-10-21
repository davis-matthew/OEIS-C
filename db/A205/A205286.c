/*
 * A205286  Number of (n+1)X2 0..2 arrays with permanents of 2X2 subblocks
 *          differing from horizontal and vertical neighbor permanents.
 * https://oeis.org/A205286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-2) +54*a(n-3) +283*a(n-4) +688*a(n-5) +1396*a(n-6)
 *            +2004*a(n-7) -212*a(n-8) -4692*a(n-9) -13221*a(n-10)
 *            -22814*a(n-11) -14278*a(n-12) +4640*a(n-13) +31376*a(n-14)
 *            +58752*a(n-15) +41064*a(n-16) +7728*a(n-17) for n>19 Initial
 *            values (n=1..19): a(1)=81, a(2)=476, a(3)=2766, a(4)=15728,...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t INIT[20] = {
    0ULL,                     /* 0 unused */
    81ULL,                    /* 1 */
    476ULL,                   /* 2 */
    2766ULL,                  /* 3 */
    15728ULL,                 /* 4 */
    89630ULL,                 /* 5 */
    511044ULL,                /* 6 */
    2913944ULL,               /* 7 */
    16611380ULL,              /* 8 */
    94699360ULL,              /* 9 */
    539880924ULL,             /* 10 */
    3077859284ULL,            /* 11 */
    17546758636ULL,           /* 12 */
    100033478620ULL,          /* 13 */
    570287962208ULL,          /* 14 */
    3251195031144ULL,         /* 15 */
    18534964466228ULL,        /* 16 */
    105667273378104ULL,       /* 17 */
    602405957212488ULL,       /* 18 */
    3434298293663288ULL       /* 19 */
};

static seq_t *cache = NULL;   /* dynamic cache for computed values */
static idx_t cache_len = 0;   /* highest index currently stored (>=0) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* First call: initialise the cache with the 19 known terms. */
    if (cache == NULL) {
        idx_t alloc = (n >= 20) ? n : 20;          /* at least 19 entries */
        cache = (seq_t *)malloc((alloc + 1) * sizeof(seq_t));
        if (cache == NULL)
            return 0;                             /* allocation failure */
        for (idx_t i = 0; i < 20; ++i)
            cache[i] = INIT[i];
        cache_len = (alloc > 19) ? 19 : alloc;
    }

    /* If the requested index is beyond what has been computed, grow
       the cache and compute missing terms. */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                                 /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* For i > 19 the recurrence applies. */
            seq_t sum = 0;
            sum += (seq_t)9     * cache[i - 2];
            sum += (seq_t)54    * cache[i - 3];
            sum += (seq_t)283   * cache[i - 4];
            sum += (seq_t)688   * cache[i - 5];
            sum += (seq_t)1396  * cache[i - 6];
            sum += (seq_t)2004  * cache[i - 7];
            sum -= (seq_t)212   * cache[i - 8];
            sum -= (seq_t)4692  * cache[i - 9];
            sum -= (seq_t)13221 * cache[i -10];
            sum -= (seq_t)22814 * cache[i -11];
            sum -= (seq_t)14278 * cache[i -12];
            sum += (seq_t)4640  * cache[i -13];
            sum += (seq_t)31376 * cache[i -14];
            sum += (seq_t)58752 * cache[i -15];
            sum += (seq_t)41064 * cache[i -16];
            sum += (seq_t)7728  * cache[i -17];
            cache[i] = sum;
        }
        cache_len = n;
    }

    return cache[n];
}
