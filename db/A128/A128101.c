/*
 * A128101  Triangle read by rows: T(n,k) is the number of ways to tile a 4 X
 *          n rectangle with k pieces of 2 X 2 tiles and 4(n-k) pieces of 1 X
 *          1 tiles (0<=k<=2*floor(n/2)).
 * https://oeis.org/A128101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128101:
   a(n) is the (r,k) entry of the triangle T(r,k) where
   T(r,k) counts tilings of a 4×r rectangle with exactly k 2×2
   tiles (and the rest 1×1 tiles).  The sequence is read row‑by‑row. */

/* --------------------------------------------------------------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* Helper tables: reachable masks and their tile counts                  */
static const int MASKS[5]      = {0, 3, 6, 12, 15};          /* 0b0000, 0011, 0110, 1100, 1111 */
static const int TILE_CNT[5]   = {0, 1, 1, 1, 2};            /* #tiles in each mask */
static int MASK_IDX[16]        = {-1};                       /* mapping mask->index */
static int MASK_IDX_INIT       = 0;

/* --------------------------------------------------------------------- */
/* Compute T(r,k) for a 4×r rectangle                                         */
static seq_t tiling_2x2_count(idx_t r, idx_t k)
{
    if (r <= 1) {                         /* width 0 or 1: only 1 tiling */
        return (k == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* maximum number of 2×2 tiles: 2*floor(r/2) */
    idx_t maxK = (r / 2) * 2;
    if (k > maxK) return (seq_t)0;

    /* initialise mask index table once */
    if (!MASK_IDX_INIT) {
        for (int i = 0; i < 16; ++i) MASK_IDX[i] = -1;
        for (int i = 0; i < 5; ++i) MASK_IDX[MASKS[i]] = i;
        MASK_IDX_INIT = 1;
    }

    size_t dim    = (size_t)maxK + 1;            /* k dimension */
    size_t totCnt = 5 * dim;                    /* 5 masks * k dimension */

    seq_t *dp_cur  = (seq_t *)calloc(totCnt, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(totCnt, sizeof(seq_t));
    if (!dp_cur || !dp_next) abort();

    /* start state: no pending tiles, 0 2×2 tiles used */
    dp_cur[0] = (seq_t)1;

    idx_t curMaxK = 0;                          /* max k reachable so far */

    for (idx_t col = 0; col < r; ++col) {
        memset(dp_next, 0, totCnt * sizeof(seq_t));

        for (int mi = 0; mi < 5; ++mi) {
            const int oldMask = MASKS[mi];
            for (size_t kk = 0; kk <= curMaxK; ++kk) {
                seq_t val = dp_cur[mi * dim + kk];
                if (!val) continue;

                if (col == r - 1) {            /* last column: cannot start new tiles */
                    dp_next[0 * dim + kk] += val;   /* keep mask 0, same k */
                } else {
                    for (int nmi = 0; nmi < 5; ++nmi) {
                        const int newMask = MASKS[nmi];
                        if (oldMask & newMask) continue;   /* overlap with pending tiles */
                        size_t nk = kk + TILE_CNT[nmi];
                        if (nk > maxK) continue;
                        dp_next[nmi * dim + nk] += val;
                    }
                }
            }
        }

        /* swap current and next DP arrays */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;

        /* update reachable k range; at most +2 tiles per column */
        if (curMaxK + 2 <= maxK) curMaxK += 2;
    }

    seq_t result = dp_cur[0 * dim + k];
    free(dp_cur);
    free(dp_next);
    return result;
}

/* --------------------------------------------------------------------- */
/* Main sequence function: a(n)                                             */
seq_t a(idx_t n)
{
    /* Find the row r and column k corresponding to index n */
    idx_t remaining = n;
    idx_t r = 0;
    idx_t k;

    while (1) {
        idx_t len = (r % 2 == 0) ? r + 1 : r;   /* length of row r */
        if (remaining < len) {
            k = remaining;
            break;
        }
        remaining -= len;
        ++r;
    }

    return tiling_2x2_count(r, k);
}
