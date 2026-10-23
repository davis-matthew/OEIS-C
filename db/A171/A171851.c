/*
 * A171851  The difference between the area under a peakless Motzkin path and
 *          the number of its U-steps, summed over all peakless Motzkin paths
 *          of length n (n>=0).
 * https://oeis.org/A171851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all peakless Motzkin paths of length n of (area
 *            under the path – number of U‑steps). The implementation uses a
 *            dynamic‑programming DP that keeps for each reachable state
 *            (height, last step was U?) the total count of paths, the total
 *            area sum, and the total number of U‑steps. After...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Maximum n expected for the tests.  Set larger if needed. */
#define MAXN 20000

/* DP arrays: (height << 1) | prevU for indexing */
static seq_t count_cur[(MAXN + 2) * 2];
static seq_t sumH_cur[(MAXN + 2) * 2];
static seq_t sumU_cur[(MAXN + 2) * 2];
static seq_t count_next[(MAXN + 2) * 2];
static seq_t sumH_next[(MAXN + 2) * 2];
static seq_t sumU_next[(MAXN + 2) * 2];

/* Pointers used for swapping current and next arrays */
static seq_t *curCount, *nextCount;
static seq_t *curSumH, *nextSumH;
static seq_t *curSumU, *nextSumU;

/* Memoised a(n) values */
static seq_t cache[MAXN + 1];
static idx_t cache_max = 0;
static int initialized = 0;

/* Extend the DP tables up to 'target' steps */
static void recompute_up_to(idx_t target)
{
    if (!initialized) {
        /* Zero all DP arrays */
        size_t total = (MAXN + 2) * 2;
        for (size_t i = 0; i < total; ++i) {
            count_cur[i] = 0;
            sumH_cur[i] = 0;
            sumU_cur[i] = 0;
            count_next[i] = 0;
            sumH_next[i] = 0;
            sumU_next[i] = 0;
        }
        /* Starting state: length 0, height 0, last not U */
        count_cur[0] = 1;
        sumH_cur[0] = 0;
        sumU_cur[0] = 0;

        cache[0] = 0;

        curCount  = count_cur;
        nextCount = count_next;
        curSumH   = sumH_cur;
        nextSumH  = sumH_next;
        curSumU   = sumU_cur;
        nextSumU  = sumU_next;

        cache_max   = 0;
        initialized = 1;
    }

    if (target <= cache_max)
        return;

    for (idx_t step = cache_max + 1; step <= target; ++step) {
        /* max height after this step */
        idx_t max_h_new = step;
        idx_t max_h_old = step - 1;

        /* Zero next arrays for the heights that will be used */
        for (idx_t h = 0; h <= max_h_new; ++h) {
            size_t base = ((size_t)h << 1);
            nextCount[base]         = 0;
            nextCount[base | 1]     = 0;
            nextSumH[base]          = 0;
            nextSumH[base | 1]      = 0;
            nextSumU[base]          = 0;
            nextSumU[base | 1]      = 0;
        }

        /* Process transitions from current states */
        for (idx_t h = 0; h <= max_h_old; ++h) {
            size_t base0 = ((size_t)h << 1);          /* prevU == 0 */
            seq_t c0 = curCount[base0];
            seq_t s0 = curSumH[base0];
            seq_t u0 = curSumU[base0];

            if (c0) {
                /* Horizontal step */
                nextCount[base0] += c0;
                nextSumH[base0]  += s0 + ((seq_t)h) * c0;
                nextSumU[base0]  += u0;

                /* Up step */
                size_t baseU = (((size_t)(h + 1) << 1) | 1);
                nextCount[baseU] += c0;
                nextSumH[baseU]  += s0 + ((seq_t)(h + 1)) * c0;
                nextSumU[baseU]  += u0 + c0;

                /* Down step – allowed only if h > 0 */
                if (h > 0) {
                    size_t baseD = (((size_t)(h - 1) << 1));
                    nextCount[baseD] += c0;
                    nextSumH[baseD]  += s0 + ((seq_t)(h - 1)) * c0;
                    nextSumU[baseD]  += u0;
                }
            }

            size_t base1 = base0 | 1;                  /* prevU == 1 */
            seq_t c1 = curCount[base1];
            seq_t s1 = curSumH[base1];
            seq_t u1 = curSumU[base1];

            if (c1) {
                /* Horizontal step (prev becomes not U) */
                nextCount[base0] += c1;
                nextSumH[base0]  += s1 + ((seq_t)h) * c1;
                nextSumU[base0]  += u1;

                /* Up step */
                size_t baseU = (((size_t)(h + 1) << 1) | 1);
                nextCount[baseU] += c1;
                nextSumH[baseU]  += s1 + ((seq_t)(h + 1)) * c1;
                nextSumU[baseU]  += u1 + c1;

                /* Down step is forbidden because previous step was U */
            }
        }

        /* Swap the arrays for the next iteration */
        seq_t *tmp;
        tmp = curCount; curCount = nextCount; nextCount = tmp;
        tmp = curSumH;  curSumH  = nextSumH;  nextSumH  = tmp;
        tmp = curSumU;  curSumU  = nextSumU;  nextSumU  = tmp;

        /* After swapping, cur* now holds data for length 'step' */
        seq_t totalH = curSumH[0];      /* height 0, prevU == 0 */
        seq_t totalU = curSumU[0];
        cache[step] = totalH - totalU;
    }

    cache_max = target;
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    /* a(0) is already set to 0 in the initialization */
    if (n == 0)
        return cache[0];
    recompute_up_to(n);
    return cache[n];
}
