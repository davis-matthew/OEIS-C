/*
 * A228284  Number of nX9 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, vertically or nw-se diagonally.
 * https://oeis.org/A228284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×9 binary matrices with top‑left cell 1 * and
 *            no two ones adjacent horizontally, vertically, or in the *
 *            northwest‑southeast diagonal. Dynamic programming over the 9‑bit
 *            * row states, with memoising of results
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#ifdef __cplusplus
#include <new>
#endif

#define MAX_MASKS 512 /* 2^9 possible row patterns */

/* All admissible 9‑bit row masks (no horizontal adjacency) */
static uint16_t masks[MAX_MASKS];
static int maskCount = 0;

/* Indexes of masks that have the leftmost bit set (top‑left must be 1) */
static int firstIdx[MAX_MASKS];
static int firstCount = 0;

/* DP arrays for the current and next rows */
static seq_t dpPrev[MAX_MASKS];
static seq_t dpNext[MAX_MASKS];

/* ---- Cache for a(n) values ---- */
static seq_t *seqCache = NULL;
static size_t cacheSize = 0;   /* allocated length of seqCache */
static idx_t cacheMax = 0;     /* highest n already computed */

/* ---- Ensure cache capacity ---- */
static void ensure_cache(size_t newSize) {
    if (newSize <= cacheSize)
        return;
#ifdef __cplusplus
    seq_t *newPtr = new seq_t[newSize];
    if (seqCache) {
        for (size_t i = 0; i < cacheSize; ++i)
            newPtr[i] = seqCache[i];
        delete[] seqCache;
    }
    seqCache = newPtr;
#else
    seq_t *newPtr = (seq_t *)realloc(seqCache, newSize * sizeof(seq_t));
    if (!newPtr) {
        abort();/* allocation failed */
    }
    seqCache = newPtr;
#endif
    cacheSize = newSize;
}

/* ---- Initialization of state tables ---- */
static void init_state(void) {
    maskCount = 0;
    firstCount = 0;
    for (uint16_t m = 0; m < 512; ++m) {
        if ((m & (m << 1)) == 0) {            /* no horizontal 1‑adjacency */
            masks[maskCount++] = m;
            if (m & 1)
                firstIdx[firstCount++] = maskCount - 1; /* top‑left must be 1 */
        }
    }

    /* initial DP for the first row */
    for (int i = 0; i < maskCount; ++i)
        dpPrev[i] = 0;
    for (int k = 0; k < firstCount; ++k)
        dpPrev[firstIdx[k]] = 1;
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n) {
    assert(n >= 1);  /* sequence defined for n >= 1 */

    if (!seqCache) {
        /* first time: set up state and cache */
        init_state();
        ensure_cache(2);          /* indices 0 and 1 */
        seq_t sum = 0;
        for (int i = 0; i < maskCount; ++i)
            sum += dpPrev[i];
        seqCache[1] = sum;
        cacheMax = 1;
    }

    if (n <= cacheMax)
        return seqCache[n];

    /* grow cache to accommodate requested n */
    ensure_cache(n + 1);

    /* extend DP from cacheMax+1 up to n */
    for (idx_t r = cacheMax + 1; r <= n; ++r) {
        /* reset next step */
        for (int i = 0; i < maskCount; ++i)
            dpNext[i] = 0;

        /* transition from current dpPrev to dpNext */
        for (int i = 0; i < maskCount; ++i) {
            seq_t val = dpPrev[i];
            if (val == 0)
                continue;
            uint16_t prevMask = masks[i];
            for (int j = 0; j < maskCount; ++j) {
                uint16_t curMask = masks[j];
                if ((curMask & prevMask) != 0)
                    continue;           /* vertical adjacency forbidden */
                if ((curMask & (prevMask << 1)) != 0)
                    continue;           /* NW‑SE diagonal forbidden */
                dpNext[j] += val;
            }
        }

        /* finish this row */
        for (int i = 0; i < maskCount; ++i)
            dpPrev[i] = dpNext[i];

        /* compute a(r) = sum of dpPrev */
        seq_t sum = 0;
        for (int i = 0; i < maskCount; ++i)
            sum += dpPrev[i];

        seqCache[r] = sum;
    }

    cacheMax = n;
    return seqCache[n];
}
