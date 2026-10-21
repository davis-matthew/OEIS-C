/*
 * A207920  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence given in the OEIS entry. The implementation
 *            counts 5×n binary arrays that avoid horizontal patterns 000 and
 *            101 and vertical patterns 010 and 101. The algorithm uses a
 *            transfer‑matrix DP over pairs of consecutive columns, where each
 *            column is represented by a 5‑bit pattern
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define PAT_COUNT 32                 /* 5‑bit patterns: 0 … 31 */
#define INITIAL_CACHE_CAP 10000      /* enough for requested n ≤ 10000 */

static seq_t *cache = NULL;          /* cache[1] … cache[n]   */
static size_t cacheCap = 0;          /* current allocated size */
static size_t cacheSize = 0;         /* maximal n cached so far */

static bool verticalOK[PAT_COUNT];
static bool allowed[PAT_COUNT][PAT_COUNT][PAT_COUNT];
static bool structsComputed = false;

static seq_t dpPrev[PAT_COUNT][PAT_COUNT];
static seq_t dpNext[PAT_COUNT][PAT_COUNT];

/* Sum all entries of a PAT_COUNT×PAT_COUNT matrix. */
static seq_t sum_matrix(const seq_t mat[PAT_COUNT][PAT_COUNT])
{
    seq_t total = 0;
    for (size_t i = 0; i < PAT_COUNT; ++i)
        for (size_t j = 0; j < PAT_COUNT; ++j)
            total += mat[i][j];
    return total;
}

/* Pre‑compute which 5‑bit columns are vertically admissible and which
   triples of columns are horizontally admissible. */
static void compute_structures(void)
{
    if (structsComputed) return;

    /* Vertical admissibility: forbid 010 or 101 in any 3 consecutive rows. */
    for (int pat = 0; pat < PAT_COUNT; ++pat) {
        bool ok = true;
        for (int r = 2; r < 5 && ok; ++r) {
            int a = (pat >> (r - 2)) & 1;
            int b = (pat >> (r - 1)) & 1;
            int c = (pat >> r) & 1;
            if ((a == 0 && b == 1 && c == 0) ||
                (a == 1 && b == 0 && c == 1))
                ok = false;
        }
        verticalOK[pat] = ok;
    }

    /* Horizontal admissibility: forbid 000 or 101 in any 3 consecutive columns
       for each of the 5 rows. */
    for (int p = 0; p < PAT_COUNT; ++p) {
        if (!verticalOK[p]) continue;
        for (int q = 0; q < PAT_COUNT; ++q) {
            if (!verticalOK[q]) continue;
            for (int k = 0; k < PAT_COUNT; ++k) {
                if (!verticalOK[k]) continue;
                bool ok = true;
                for (int r = 0; r < 5; ++r) {
                    int bp = (p >> r) & 1;
                    int bq = (q >> r) & 1;
                    int bk = (k >> r) & 1;
                    if ((bp == 0 && bq == 0 && bk == 0) ||
                        (bp == 1 && bq == 0 && bk == 1)) {
                        ok = false;
                        break;
                    }
                }
                allowed[p][q][k] = ok;
            }
        }
    }

    structsComputed = true;
}

/* Ensure that the cache has room for at least n elements. */
static void ensure_cache_cap(idx_t n)
{
    if (n < cacheCap) return;
    size_t newCap = (n > cacheCap * 2) ? n + 1 : cacheCap * 2 + 1;
    seq_t *newCache = (seq_t *)malloc(newCap * sizeof(seq_t));
    if (!newCache) return;            /* allocation failure → silently fail */
    for (size_t i = 1; i <= cacheSize; ++i) newCache[i] = cache[i];
    free(cache);
    cache = newCache;
    cacheCap = newCap;
}

/* Compute a(1) and a(2) and initialize dpPrev for n = 2. */
static void init_initial_terms(void)
{
    compute_structures();

    seq_t verticalCount = 0;
    for (int i = 0; i < PAT_COUNT; ++i)
        if (verticalOK[i]) ++verticalCount;

    cache[1] = verticalCount;           /* a(1) */
    /* dpPrev for length 2 */
    for (int i = 0; i < PAT_COUNT; ++i)
        for (int j = 0; j < PAT_COUNT; ++j)
            dpPrev[i][j] = (verticalOK[i] && verticalOK[j]) ? (seq_t)1 : (seq_t)0;

    cache[2] = sum_matrix(dpPrev);      /* a(2) */
    cacheSize = 2;
}

/* Extend the cache from cacheSize+1 up to n. */
static void extend_cache(idx_t n)
{
    if (n <= cacheSize) return;
    ensure_cache_cap((size_t)n);

    for (idx_t step = cacheSize + 1; step <= n; ++step) {
        /* Build dpNext for arrays of length step. */
        for (int i = 0; i < PAT_COUNT; ++i)
            for (int j = 0; j < PAT_COUNT; ++j)
                dpNext[i][j] = (seq_t)0;

        for (int p = 0; p < PAT_COUNT; ++p) {
            for (int q = 0; q < PAT_COUNT; ++q) {
                seq_t val = dpPrev[p][q];
                if (!val) continue;
                for (int k = 0; k < PAT_COUNT; ++k) {
                    if (allowed[p][q][k]) {
                        dpNext[q][k] += val;
                    }
                }
            }
        }

        cache[step] = sum_matrix(dpNext);
        /* Prepare dpPrev for the next step. */
        for (int i = 0; i < PAT_COUNT; ++i)
            for (int j = 0; j < PAT_COUNT; ++j)
                dpPrev[i][j] = dpNext[i][j];
    }
    cacheSize = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    if (cache == NULL) {
        cacheCap = INITIAL_CACHE_CAP + 1;
        cache = (seq_t *)malloc(cacheCap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
    }

    if (cacheSize == 0) init_initial_terms();

    if ((size_t)n > cacheSize) extend_cache((idx_t)n);

    return cache[n];
}
