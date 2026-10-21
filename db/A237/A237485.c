/*
 * A237485  Number of (n+1)X(4+1) 0..3 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A237485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 21,
 *            big any n
 * Helpers:   subblock_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237485: Number of (n+1)×5 arrays over {0,1,2,3} such that
   max + min – lower‑median of every 2×2 subblock is a constant across the
   whole array.  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/subblock_k.h"

#define STATES 1024   /* 4^5 */

static bool built = false;

/* adjacency data for each K (0..3) */
static uint32_t *offsetK[4];
static uint16_t *neighborsK[4];

/* base DP counts for arrays of height 2 (n=1) */
static seq_t dp_base[4][STATES];
/* current and next DP rows for heights > 2 */
static seq_t *dp_prev[4];
static seq_t *dp_curr[4];

/* cache of sequence values */
static seq_t *cache = NULL;
static idx_t cache_allocated = 0;
static idx_t cache_highest = 0;

/* initialise adjacency lists and base DP counts */
static void init_adjacency(void)
{
    int i, j, k, x;
    /* digits of each state in base 4 */
    static uint8_t digits[STATES][5];
    for (i = 0; i < STATES; ++i) {
        int tmp = i;
        for (j = 0; j < 5; ++j) {
            digits[i][j] = (uint8_t)(tmp & 3);
            tmp >>= 2;
        }
    }

    /* count neighbours for each K */
    static int cntK[4][STATES];
    memset(cntK, 0, sizeof(cntK));

    for (i = 0; i < STATES; ++i) {
        for (j = 0; j < STATES; ++j) {
            int kval = -1;
            for (x = 0; x < 4; ++x) {
                int a0 = digits[i][x];
                int a1 = digits[i][x + 1];
                int b0 = digits[j][x];
                int b1 = digits[j][x + 1];
                int ktmp = subblock_k(a0, a1, b0, b1);
                if (kval == -1) kval = ktmp;
                else if (kval != ktmp) { kval = -2; break; }
            }
            if (kval >= 0) cntK[kval][i]++;
        }
    }

    /* allocate offsets and neighbours lists */
    for (k = 0; k < 4; ++k) {
        offsetK[k] = (uint32_t*)malloc((STATES + 1) * sizeof(uint32_t));
        if (!offsetK[k]) abort();
        offsetK[k][0] = 0;
        for (i = 0; i < STATES; ++i)
            offsetK[k][i + 1] = offsetK[k][i] + cntK[k][i];
        neighborsK[k] = (uint16_t*)malloc(offsetK[k][STATES] * sizeof(uint16_t));
        if (!neighborsK[k]) abort();
    }

    /* second pass: fill neighbour lists */
    uint16_t *p[4];
    for (k = 0; k < 4; ++k) p[k] = neighborsK[k];
    for (i = 0; i < STATES; ++i) {
        for (j = 0; j < STATES; ++j) {
            int kval = -1;
            for (x = 0; x < 4; ++x) {
                int a0 = digits[i][x];
                int a1 = digits[i][x + 1];
                int b0 = digits[j][x];
                int b1 = digits[j][x + 1];
                int ktmp = subblock_k(a0, a1, b0, b1);
                if (kval == -1) kval = ktmp;
                else if (kval != ktmp) { kval = -2; break; }
            }
            if (kval >= 0) {
                *(p[kval]) = (uint16_t)j;
                p[kval]++;
            }
        }
    }

    /* base DP counts for height 2 (n=1) */
    for (k = 0; k < 4; ++k)
        for (i = 0; i < STATES; ++i) dp_base[k][i] = 0;
    for (k = 0; k < 4; ++k) {
        for (i = 0; i < STATES; ++i) {
            uint32_t start = offsetK[k][i];
            uint32_t end   = offsetK[k][i + 1];
            uint16_t *list = neighborsK[k] + start;
            for (uint32_t idx = start; idx < end; ++idx)
                dp_base[k][list[idx - start]]++;
        }
    }

    /* allocate DP working arrays */
    for (k = 0; k < 4; ++k) {
        dp_prev[k] = (seq_t*)malloc(STATES * sizeof(seq_t));
        dp_curr[k] = (seq_t*)malloc(STATES * sizeof(seq_t));
        if (!dp_prev[k] || !dp_curr[k]) abort();
    }

    /* initialise dp_prev with base counts */
    for (k = 0; k < 4; ++k)
        for (i = 0; i < STATES; ++i)
            dp_prev[k][i] = dp_base[k][i];

    built = true;
}

/* ensure cache array big enough */
static void ensure_cache(idx_t n)
{
    if (cache_allocated > n) return;
    size_t needed = (size_t)n + 1;                /* indices 0..n */
    seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_allocated = (idx_t)needed;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                   /* offset 1, undefined below */
    if (!built) init_adjacency();
    ensure_cache(n);

    if (n <= cache_highest) return cache[(size_t)n];

    if (cache_highest == 0) {                    /* compute first term a(1) */
        seq_t total1 = 0;
        for (int k = 0; k < 4; ++k)
            for (int i = 0; i < STATES; ++i)
                total1 += dp_prev[k][i];
        cache[1] = total1;
        cache_highest = 1;
    }

    for (idx_t cur = cache_highest; cur < n; ++cur) {
        /* compute dp_curr from dp_prev for next height */
        for (int k = 0; k < 4; ++k)
            memset(dp_curr[k], 0, STATES * sizeof(seq_t));

        for (int k = 0; k < 4; ++k) {
            for (int i = 0; i < STATES; ++i) {
                seq_t val = dp_prev[k][i];
                if (!val) continue;
                uint32_t start = offsetK[k][i];
                uint32_t end   = offsetK[k][i + 1];
                uint16_t *list = neighborsK[k] + start;
                for (uint32_t idx = start; idx < end; ++idx) {
                    uint16_t j = list[idx - start];
                    dp_curr[k][j] += val;
                }
            }
        }

        /* total for array height cur+2 (i.e., n = cur+1) */
        idx_t nextn = cur + 1;
        seq_t total = 0;
        for (int k = 0; k < 4; ++k)
            for (int i = 0; i < STATES; ++i)
                total += dp_curr[k][i];
        cache[(size_t)nextn] = total;

        /* swap dp_prev and dp_curr for next iteration */
        for (int k = 0; k < 4; ++k) {
            seq_t *tmp = dp_prev[k];
            dp_prev[k] = dp_curr[k];
            dp_curr[k] = tmp;
        }
    }

    cache_highest = n;
    return cache[(size_t)n];
}
