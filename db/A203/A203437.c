/*
 * A203437  Number of nX2 0..7 arrays with every nonzero element less than or
 *          equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A203437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A203437
 *  Number of n×2 arrays over {0,…,7} in which every non‑zero entry
 *  is ≤ at least two of its horizontal and vertical neighbours.
 *
 *  The algorithm is a dynamic programming over rows.
 *  A state is a pair of consecutive rows (row_{k‑1},row_k).
 *  Transition to a new row r defines state (row_k,r) and
 *  is allowed iff row_k satisfies the neighbour condition with
 *  its two neighbours (row_{k‑1} and r).  After n rows the last row
 *  is checked with a bottom sentinel row of zeros.
 */

#include <stdbool.h>

/* Precomputed tables ------------------------------------------------- */

static int rowVals[64][2];                     /* decode a row code to two cells 0..7 */
static unsigned char adjCnt[4096];             /* number of admissible next rows */
static unsigned char adj[4096][64];            /* list of next rows for each pair */
static bool initialized = false;

/* Decode the 2‑cell row from a base‑8 code (0..63). */
static void init_rows(void)
{
    for (int code = 0; code < 64; ++code) {
        int v0 = code / 8;
        int v1 = code % 8;
        rowVals[code][0] = v0;            /* values are 0..7 */
        rowVals[code][1] = v1;
    }
}

/* Return true if all non‑zero cells in the current row satisfy
 * the condition with neighbours abovec (above) and belowc
 * (below) and the horizontal neighbour in the same row.
 */
static inline bool check_rows(int abovec, int curc, int belowc)
{
    const int *above = rowVals[abovec];
    const int *cur   = rowVals[curc];
    const int *below = rowVals[belowc];
    for (int col = 0; col < 2; ++col) {
        int val = cur[col];
        if (val == 0) continue;
        int cnt = 0;
        if (cur[1 - col] >= val) ++cnt;   /* horizontal neighbour */
        if (above[col] >= val) ++cnt;     /* vertical neighbour above */
        if (below[col] >= val) ++cnt;     /* vertical neighbour below */
        if (cnt < 2) return false;
    }
    return true;
}

/* Precompute the adjacency lists for all pairs of rows. */
static void init_adj(void)
{
    init_rows();
    for (int pp = 0; pp < 64; ++pp) {
        for (int p = 0; p < 64; ++p) {
            int idx = pp * 64 + p;
            unsigned char cnt = 0;
            for (int curr = 0; curr < 64; ++curr) {
                if (check_rows(pp, p, curr)) {
                    adj[idx][cnt++] = (unsigned char)curr;
                }
            }
            adjCnt[idx] = cnt;
        }
    }
}

/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (!initialized) {
        init_adj();
        initialized = true;
    }

    /* DP arrays: cur and next hold counts for each state pair. */
    static seq_t dpCur[4096];
    static seq_t dpNext[4096];
    seq_t *cur  = dpCur;
    seq_t *next = dpNext;

    /* Initialise: only the starting state (sentinel, sentinel). */
    for (int i = 0; i < 4096; ++i) cur[i] = 0;
    cur[0] = 1;  /* code 0 corresponds to a row of two zeros */

    /* Process each of the n rows. */
    for (idx_t row = 0; row < n; ++row) {
        /* zero next array */
        for (int i = 0; i < 4096; ++i) next[i] = 0;

        /* Transition over all current states. */
        for (int pair = 0; pair < 4096; ++pair) {
            seq_t val = cur[pair];
            if (val == 0) continue;
            int p = pair % 64;                     /* current row in the pair */
            unsigned char cnt = adjCnt[pair];
            unsigned char *list = adj[pair];
            for (unsigned char k = 0; k < cnt; ++k) {
                int curr = list[k];
                int nextPair = p * 64 + curr;      /* new state (prev=curr, new=next) */
                next[nextPair] += val;
            }
        }
        /* swap cur and next for next iteration */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* After n rows, the last row has not been checked yet.
       Add a bottom sentinel row (code 0) to finish the check. */
    seq_t result = 0;
    for (int pair = 0; pair < 4096; ++pair) {
        seq_t val = cur[pair];
        if (val == 0) continue;
        int pp = pair / 64;  /* previous row */
        int p  = pair % 64;  /* current (last) row */
        if (check_rows(pp, p, 0)) {          /* bottom sentinel */
            result += val;
        }
    }
    return result;
}
