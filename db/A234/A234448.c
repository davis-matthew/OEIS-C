/*
 * A234448  Number of (n+1) X (5+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of (n+1)×6 arrays with entries 0..4 such that
   for every adjacent 2×2 subblock the difference of its diagonal sum
   and antidiagonal sum is ±1.  We count all such arrays by dynamic
   programming over the difference vectors of adjacent columns. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

extern seq_t seq_mul(seq_t a, seq_t b);  /* unused, kept for compatibility */

/* --------------------------------------------------------------------- */
/*  Definition of the state space  (difference vectors)                 */
/* --------------------------------------------------------------------- */

/* each difference ∈ [−4,4] → 9 possibilities, 5 coordinates                 */
#define MAX_COORD 9
#define MAX_DDIM  5
/* Each coordinate can be incremented or decremented: 2^5 possible neighbours */
#define MAX_NEIGH 32
/* Upper bound on number of states: all 5‑tuples of values in [−4,4] */
#define MAX_R     59049   /* 9^5 */

/* state information */
typedef struct {
    int d[MAX_DDIM];          /* difference vector */
    unsigned int count;       /* number of admissible starting offsets in 0..4 */
} DeltaInfo;

/* storage of all states */
static DeltaInfo deltaList[MAX_R];
static size_t R = 0;

/* mapping from packed index to state index; -1 means invalid */
static int idxMap[MAX_R];

/* adjacency lists: neighbours that differ by ±1 in each coordinate */
static uint16_t neighList[MAX_R][MAX_NEIGH];
static uint8_t  neighCnt[MAX_R];

/* --------------------------------------------------------------------- */
/*  Helpers                                                            */
/* --------------------------------------------------------------------- */

/* pack a difference vector into a base‑9 index */
static inline int pack(const int d[MAX_DDIM])
{
    int idx = 0;
    for (int i = 0; i < MAX_DDIM; ++i)
        idx = idx * MAX_COORD + (d[i] + 4);   /* shift to 0..8 */
    return idx;
}

/* initialise state list and adjacency graph (called once) */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* initialise map to -1 */
    for (int i = 0; i < MAX_R; ++i) idxMap[i] = -1;

    /* enumerate all 5‑tuples of differences in [−4,4] */
    int d[MAX_DDIM];
    for (int a0 = 0; a0 < MAX_COORD; ++a0) {
        d[0] = a0 - 4;
        for (int a1 = 0; a1 < MAX_COORD; ++a1) {
            d[1] = a1 - 4;
            for (int a2 = 0; a2 < MAX_COORD; ++a2) {
                d[2] = a2 - 4;
                for (int a3 = 0; a3 < MAX_COORD; ++a3) {
                    d[3] = a3 - 4;
                    for (int a4 = 0; a4 < MAX_COORD; ++a4) {
                        d[4] = a4 - 4;

                        /* compute cumulative sums and their range */
                        int cum = 0, maxi = 0, mini = 0;
                        for (int j = 0; j < MAX_DDIM; ++j) {
                            cum += d[j];
                            if (cum > maxi) maxi = cum;
                            if (cum < mini) mini = cum;
                        }
                        int range = maxi - mini;
                        if (range <= 4) {
                            unsigned int cnt = 5 - range;   /* possible offsets 0..4 */
                            deltaList[R].count = cnt;
                            for (int j = 0; j < MAX_DDIM; ++j)
                                deltaList[R].d[j] = d[j];
                            idxMap[pack(d)] = (int)R;
                            ++R;
                        }
                    }
                }
            }
        }
    }

    /* build adjacency lists: neighbours differ by ±1 in each coordinate */
    int tmp[MAX_DDIM];
    for (size_t i = 0; i < R; ++i) {
        const int *cur = deltaList[i].d;
        for (int mask = 0; mask < (1 << MAX_DDIM); ++mask) {
            bool ok = true;
            for (int k = 0; k < MAX_DDIM; ++k) {
                int delta = ((mask >> k) & 1) ? 1 : -1;
                int val = cur[k] + delta;
                if (val < -4 || val > 4) { ok = false; break; }
                tmp[k] = val;
            }
            if (!ok) continue;
            int idx = idxMap[pack(tmp)];
            if (idx < 0) continue;   /* state has range >4 */
            neighCnt[i]++;             /* <=32 */
            neighList[i][neighCnt[i] - 1] = (uint16_t)idx;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();                     /* ensure the state graph exists */

    if (n < 1) return (seq_t)0; /* offset 1; behaviour below is irrelevant */

    size_t steps = (size_t)n;   /* number of transitions between rows */

    /* allocate DP arrays (use new in C++ and malloc in C) */
    seq_t *dpPrev;
    seq_t *dpCurr;
#ifdef __cplusplus
    dpPrev = new seq_t[R];
    dpCurr = new seq_t[R];
    if (!dpPrev || !dpCurr) { delete[] dpPrev; delete[] dpCurr; return (seq_t)0; }
#else
    dpPrev = (seq_t *)malloc(R * sizeof(seq_t));
    dpCurr = (seq_t *)malloc(R * sizeof(seq_t));
    if (!dpPrev || !dpCurr) { free(dpPrev); free(dpCurr); return (seq_t)0; }
#endif

    /* initial row: each state contributes its number of possible offsets */
    for (size_t i = 0; i < R; ++i)
        dpPrev[i] = (seq_t)deltaList[i].count;

    /* transition through all remaining rows */
    for (size_t step = 1; step <= steps; ++step) {
        /* clear current DP */
        for (size_t i = 0; i < R; ++i)
            dpCurr[i] = (seq_t)0;

        /* propagate counts to neighbours */
        for (size_t i = 0; i < R; ++i) {
            seq_t val = dpPrev[i];
            if (!val) continue;
            uint8_t nc = neighCnt[i];
            for (uint8_t k = 0; k < nc; ++k) {
                uint16_t j = neighList[i][k];
                dpCurr[j] += val;
            }
        }

        /* multiply by the number of admissible offsets for the new row */
        for (size_t i = 0; i < R; ++i) {
            unsigned int cnt = deltaList[i].count;
            if (cnt > 1) dpCurr[i] = dpCurr[i] * (seq_t)cnt;
        }

        /* swap dpPrev and dpCurr for next iteration */
        seq_t *tmp = dpPrev;
        dpPrev = dpCurr;
        dpCurr = tmp;
    }

    /* sum all ways over last row states */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < R; ++i)
        result += dpPrev[i];

    /* free allocated memory */
#ifdef __cplusplus
    delete[] dpPrev;
    delete[] dpCurr;
#else
    free(dpPrev);
    free(dpCurr);
#endif

    return result;
}
