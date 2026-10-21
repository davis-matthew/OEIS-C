/*
 * A224189  Number of nX7 0..2 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A224189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 53, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Dynamic programming over the poset of unimodal length‑7
   0..2 vectors.  a(n) counts the number of chains of length n
   (with repetitions) in this poset, i.e. the number of n×7 arrays
   whose rows are unimodal and whose columns are non‑decreasing. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define LENGTH 7
#define MAX_STATES 240          /* 239 + safety */
#define MAX_EDGES 200000        /* ample space for edges */

/* State representation (local to this file) */
static uint8_t rows[MAX_STATES][LENGTH];
static uint16_t succ_head[MAX_STATES];
static uint16_t succ_to[MAX_EDGES];
static uint16_t succ_next[MAX_EDGES];
static uint16_t succ_edge_count;
static uint16_t stateCount;

/* DP containers */
static seq_t *dpCurr = NULL;
static seq_t *dpNext = NULL;

/* Cache for computed a(n) values */
static seq_t *aCache = NULL;
static idx_t cacheSize = 0;        /* maximum n currently computed (>=1) */
static idx_t cacheCap = 0;

/* Flag whether initialization has been done */
static bool initialized = false;

/* ---------- helpers ---------------------------------------------------- */

static bool is_unimodal(const uint8_t *v)
{
    bool decreasing = false;
    for (int i = 1; i < LENGTH; ++i) {
        if (!decreasing) {
            if (v[i] > v[i - 1])
                continue;
            if (v[i] < v[i - 1])
                decreasing = true;   /* peak reached */
        } else { /* already decreasing */
            if (v[i] > v[i - 1])
                return false;        /* increase after decrease */
        }
    }
    return true;
}

/* Build all unimodal states and their adjacency lists */
static void init_poset(void)
{
    /* ---------- generate states ---------------------------------------- */
    uint32_t total = 1;
    for (int i = 0; i < LENGTH; ++i) total *= 3;   /* 3^7 = 2187 */

    stateCount = 0;
    for (uint32_t num = 0; num < total; ++num) {
        uint32_t x = num;
        for (int j = 0; j < LENGTH; ++j) {
            rows[stateCount][j] = x % 3;
            x /= 3;
        }
        if (is_unimodal(rows[stateCount]))
            ++stateCount;
    }

    /* ---------- adjacency --------------------------------------------- */
    succ_edge_count = 0;
    for (uint16_t i = 0; i < stateCount; ++i)
        succ_head[i] = 0xFFFF;            /* sentinel */

    for (uint16_t i = 0; i < stateCount; ++i) {
        for (uint16_t j = 0; j < stateCount; ++j) {
            bool le = true;
            for (int k = 0; k < LENGTH; ++k) {
                if (rows[i][k] > rows[j][k]) { le = false; break; }
            }
            if (le) {
                succ_to[succ_edge_count]   = j;
                succ_next[succ_edge_count] = succ_head[i];
                succ_head[i]               = succ_edge_count;
                ++succ_edge_count;
            }
        }
    }

    /* ---------- DP arrays -------------------------------------------- */
    dpCurr = (seq_t *)malloc(stateCount * sizeof(seq_t));
    dpNext = (seq_t *)malloc(stateCount * sizeof(seq_t));
    for (uint16_t i = 0; i < stateCount; ++i)
        dpCurr[i] = (seq_t)1;           /* chains of length 1 */

    /* ---------- caching ------------------------------------------------ */
    cacheCap = 1024;                     /* initial cache capacity */
    aCache = (seq_t *)malloc(cacheCap * sizeof(seq_t));
    cacheSize = 1;                      /* a(1) will be stored at index 1 */

    /* compute a(1) */
    seq_t sum = 0;
    for (uint16_t i = 0; i < stateCount; ++i)
        sum += dpCurr[i];
    aCache[1] = sum;

    initialized = true;
}

/* ---------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init_poset();

    /* enlarge cache if needed */
    if (n > cacheCap - 1) {
        while (n > cacheCap - 1)
            cacheCap <<= 1;
        aCache = (seq_t *)realloc(aCache, cacheCap * sizeof(seq_t));
    }

    /* return cached value when possible */
    if (n <= cacheSize)
        return aCache[n];

    /* iterative DP from cacheSize+1 up to n */
    for (idx_t step = cacheSize + 1; step <= n; ++step) {
        /* clear next DP */
        for (uint16_t i = 0; i < stateCount; ++i)
            dpNext[i] = (seq_t)0;

        /* transition: for each predecessor s add dpCurr[s] to all successors */
        for (uint16_t sIdx = 0; sIdx < stateCount; ++sIdx) {
            seq_t val = dpCurr[sIdx];
            if (val == 0)
                continue;
            uint16_t e = succ_head[sIdx];
            while (e != 0xFFFF) {
                uint16_t tIdx = succ_to[e];
                dpNext[tIdx] += val;
                e = succ_next[e];
            }
        }

        /* compute sum for a(step) */
        seq_t sum = 0;
        for (uint16_t i = 0; i < stateCount; ++i)
            sum += dpNext[i];
        aCache[step] = sum;

        /* swap DP buffers */
        seq_t *tmp = dpCurr;
        dpCurr = dpNext;
        dpNext = tmp;

        cacheSize = step;
    }

    return aCache[n];
}
