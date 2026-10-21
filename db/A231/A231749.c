/*
 * A231749  Number of nX4 0..2 arrays with no element less than a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors.
 * https://oeis.org/A231749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes A231749: the number of n×4 arrays over {0,1,2} with
   no element less than a strict majority of its horizontal, vertical,
   diagonal and antidiagonal neighbors.  The algorithm is
   dynamic‑programming over the two most recent rows. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* constants */
#define ROWS   4              /* width of the grid */
#define ALPH   3              /* values 0,1,2 */
#define PATTS  (ALPH * ALPH * ALPH * ALPH)   /* 81 patterns per row */
#define STATE  (PATTS * PATTS)               /* 6561 two‑row states */

/* precomputed tables */
static int patval[PATTS][ROWS];
static uint8_t validStart[STATE];
static uint8_t validFinal[STATE];
static uint8_t transCount[STATE];
static uint8_t trans[STATE][PATTS];

/* helper: compute the rule for row0 when only row1 is below */
static bool rowValidStart(int idx0, int idx1) {
    const int *r0 = patval[idx0];
    const int *r1 = patval[idx1];
    for (int j = 0; j < ROWS; ++j) {
        int val = r0[j];
        int neigh = 0, greater = 0;

        /* left neighbour */
        if (j > 0) { ++neigh; if (r0[j-1] > val) ++greater; }
        /* right neighbour */
        if (j < ROWS-1) { ++neigh; if (r0[j+1] > val) ++greater; }

        /* down neighbour */
        ++neigh; if (r1[j] > val) ++greater;

        /* down‑left diagonal */
        if (j > 0) { ++neigh; if (r1[j-1] > val) ++greater; }
        /* down‑right diagonal */
        if (j < ROWS-1) { ++neigh; if (r1[j+1] > val) ++greater; }

        if (greater > neigh/2) return false;
    }
    return true;
}

/* helper: compute the rule for the last row, given only the row above */
static bool rowValidFinal(int idx0, int idx1) {
    const int *up   = patval[idx0];
    const int *cur  = patval[idx1];
    for (int j = 0; j < ROWS; ++j) {
        int val = cur[j];
        int neigh = 0, greater = 0;

        /* up neighbour */
        ++neigh; if (up[j] > val) ++greater;

        /* left/right neighbours */
        if (j > 0) { ++neigh; if (cur[j-1] > val) ++greater; }
        if (j < ROWS-1) { ++neigh; if (cur[j+1] > val) ++greater; }

        /* up‑left diagonal */
        if (j > 0) { ++neigh; if (up[j-1] > val) ++greater; }
        /* up‑right diagonal */
        if (j < ROWS-1) { ++neigh; if (up[j+1] > val) ++greater; }

        if (greater > neigh/2) return false;
    }
    return true;
}

/* helper: compute the middle‑row rule when we know all three rows */
static bool rowValidMiddle(int idxPrev, int idxCurr, int idxNext) {
    const int *prev = patval[idxPrev];
    const int *curr = patval[idxCurr];
    const int *next = patval[idxNext];
    for (int j = 0; j < ROWS; ++j) {
        int val = curr[j];
        int neigh = 0, greater = 0;

        /* left/right neighbours from current row */
        if (j > 0) { ++neigh; if (curr[j-1] > val) ++greater; }
        if (j < ROWS-1) { ++neigh; if (curr[j+1] > val) ++greater; }

        /* up neighbour */
        ++neigh; if (prev[j] > val) ++greater;

        /* down neighbour */
        ++neigh; if (next[j] > val) ++greater;

        /* up‑diagonals */
        if (j > 0) { ++neigh; if (prev[j-1] > val) ++greater; }
        if (j < ROWS-1) { ++neigh; if (prev[j+1] > val) ++greater; }

        /* down‑diagonals */
        if (j > 0) { ++neigh; if (next[j-1] > val) ++greater; }
        if (j < ROWS-1) { ++neigh; if (next[j+1] > val) ++greater; }

        if (greater > neigh/2) return false;
    }
    return true;
}

/* one‑time global initialization of tables */
static void init(void) {
    static bool done = false;
    if (done) return;
    done = true;

    /* pattern table */
    for (int p = 0; p < PATTS; ++p) {
        int tmp = p;
        for (int j = ROWS-1; j >= 0; --j) {
            patval[p][j] = tmp % ALPH;
            tmp /= ALPH;
        }
    }

    /* validity tables and transition graph */
    for (int idx0 = 0; idx0 < PATTS; ++idx0) {
        for (int idx1 = 0; idx1 < PATTS; ++idx1) {
            int stateIdx = idx0 * PATTS + idx1;
            validStart[stateIdx] = rowValidStart(idx0, idx1) ? 1 : 0;
            validFinal[stateIdx] = rowValidFinal(idx0, idx1) ? 1 : 0;

            uint8_t cnt = 0;
            for (int idx2 = 0; idx2 < PATTS; ++idx2) {
                if (rowValidMiddle(idx0, idx1, idx2)) {
                    trans[stateIdx][cnt++] = (uint8_t)idx2;
                }
            }
            transCount[stateIdx] = cnt;
        }
    }
}

/* DP arrays (allocated once, reused for each call) */
static seq_t cur[STATE];
static seq_t next[STATE];

/* helper: single‑row rule (no vertical neighbours) for a(1) */
static bool rowValidSingle(int idx) {
    const int *r = patval[idx];
    for (int j = 0; j < ROWS; ++j) {
        int val = r[j];
        int neigh = 0, greater = 0;
        if (j > 0) { ++neigh; if (r[j-1] > val) ++greater; }
        if (j < ROWS-1) { ++neigh; if (r[j+1] > val) ++greater; }
        if (greater > neigh/2) return false;
    }
    return true;
}

/* the requested sequence function */
seq_t a(idx_t n) {
    init();

    /* offset handling */
    if (n == 0) return (seq_t)0;

    /* a(1) */
    if (n == 1) {
        seq_t cnt = 0;
        for (int idx = 0; idx < PATTS; ++idx) {
            if (rowValidSingle(idx)) ++cnt;
        }
        return cnt;
    }

    /* a(2) */
    if (n == 2) {
        seq_t sum = 0;
        for (int idx = 0; idx < STATE; ++idx) {
            if (validStart[idx] && validFinal[idx]) sum += (seq_t)1;
        }
        return sum;
    }

    /* initialise DP for the first two rows */
    memset(cur, 0, sizeof(cur));
    for (int idx0 = 0; idx0 < PATTS; ++idx0) {
        for (int idx1 = 0; idx1 < PATTS; ++idx1) {
            if (validStart[idx0 * PATTS + idx1]) {
                cur[idx0 * PATTS + idx1] = (seq_t)1;
            }
        }
    }

    /* add rows 2 .. n-1 */
    for (idx_t step = 2; step < n; ++step) {
        memset(next, 0, sizeof(next));
        for (int state = 0; state < STATE; ++state) {
            seq_t ways = cur[state];
            if (!ways) continue;
            int cb = state % PATTS;
            uint8_t cnt = transCount[state];
            for (uint8_t k = 0; k < cnt; ++k) {
                int nc = trans[state][k];
                int nextState = cb * PATTS + nc;
                next[nextState] += ways;
            }
        }
        memcpy(cur, next, sizeof(cur));
    }

    /* final row validity */
    seq_t result = 0;
    for (int state = 0; state < STATE; ++state) {
        if (!validFinal[state]) continue;
        seq_t ways = cur[state];
        if (ways) result += ways;
    }
    return result;
}
