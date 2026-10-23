/*
 * A196317  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 1,3,0,2,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays 0..4 satisfying local neighbor
 *            counts. Computed by dynamic programming over rows with a state
 *            (row triple, mask), where mask indicates for each column whether
 *            the cell below must be equal to the cell's target value to
 *            complete its neighbor count
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility constants */
#define ROWS   3
#define STATES 125        /* 5^3 possible row configurations */
#define MASKS  8          /* 3-bit down‑requirement mask */

/* Transition tables */
static int rowVal[STATES][ROWS];      /* decoded row values */
static int nextList[STATES * MASKS][200];
static unsigned char nextCount[STATES * MASKS];

/* Problem data */
static const int target[5] = {1, 3, 0, 2, 4};
static const int reqCnt[5] = {0, 1, 2, 3, 4};

/* Helper to compute array index */
static inline int stateIndex(int triple, int mask) { return triple * MASKS + mask; }

/* Pre‑decode all row configurations */
static void initRowVal(void) {
    for (int a = 0; a < 5; ++a)
        for (int b = 0; b < 5; ++b)
            for (int c = 0; c < 5; ++c) {
                int idx = (a * 5 + b) * 5 + c;
                rowVal[idx][0] = a;
                rowVal[idx][1] = b;
                rowVal[idx][2] = c;
            }
}

/* Compute down‑requirement mask for a row with no up neighbour */
static int computeMaskFirstRow(int triple) {
    int mask = 0;
    for (int col = 0; col < ROWS; ++col) {
        int vCurr = rowVal[triple][col];
        int tCurr = target[vCurr];
        int req  = reqCnt[vCurr];
        int cnt  = 0;

        /* Only left/right neighbours exist */
        if (col > 0 && rowVal[triple][col-1] == tCurr) ++cnt;
        if (col < ROWS-1 && rowVal[triple][col+1] == tCurr) ++cnt;

        if (cnt > req || cnt < req-1) return -1;
        if (cnt == req-1) mask |= 1 << col;   /* needs target below */
    }
    return mask;
}

/* Compute down‑requirement mask after knowing the row above */
static int computeMaskNext(int prevTriple, int currTriple) {
    int mask = 0;
    for (int col = 0; col < ROWS; ++col) {
        int vCurr = rowVal[currTriple][col];
        int tCurr = target[vCurr];
        int req   = reqCnt[vCurr];
        int cnt   = 0;

        /* Up neighbour */
        if (rowVal[prevTriple][col] == tCurr) ++cnt;

        /* Left/right neighbours */
        if (col > 0 && rowVal[currTriple][col-1] == tCurr) ++cnt;
        if (col < ROWS-1 && rowVal[currTriple][col+1] == tCurr) ++cnt;

        if (cnt > req || cnt < req-1) return -1;
        if (cnt == req-1) mask |= 1 << col;   /* needs target below */
    }
    return mask;
}

/* Build transition table */
static void buildTransitions(void) {
    for (int pt = 0; pt < STATES; ++pt)
        for (int pm = 0; pm < MASKS; ++pm) {
            int pIdx = stateIndex(pt, pm);
            unsigned char cnt = 0;
            for (int ct = 0; ct < STATES; ++ct) {
                /* Check that previous row's down‑requirements are met */
                bool ok = true;
                for (int col = 0; col < ROWS; ++col) {
                    int vPrev = rowVal[pt][col];
                    int tPrev = target[vPrev];
                    int vCur  = rowVal[ct][col];
                    if (pm & (1 << col)) {
                        if (vCur != tPrev) { ok = false; break; }
                    } else {
                        if (vCur == tPrev) { ok = false; break; }
                    }
                }
                if (!ok) continue;

                int mask = computeMaskNext(pt, ct);
                if (mask < 0) continue;

                nextList[pIdx][cnt++] = stateIndex(ct, mask);
            }
            nextCount[pIdx] = cnt;
        }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset starts at 1 */

    static bool ready = false;
    static seq_t dpPrev[STATES * MASKS];
    static seq_t dpNext[STATES * MASKS];

    if (!ready) {
        initRowVal();
        buildTransitions();
        ready = true;
    }

    /* Clear previous DP state */
    for (int i = 0; i < STATES * MASKS; ++i) dpPrev[i] = 0;

    /* First row initialization */
    for (int triple = 0; triple < STATES; ++triple) {
        int mask = computeMaskFirstRow(triple);
        if (mask < 0) continue;
        dpPrev[stateIndex(triple, mask)] = 1;
    }

    if (n == 1) {
        seq_t ans = 0;
        for (int i = 0; i < STATES * MASKS; ++i)
            if ((i & 7) == 0) ans += dpPrev[i];
        return ans;
    }

    /* Process remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear dpNext */
        for (int i = 0; i < STATES * MASKS; ++i) dpNext[i] = 0;

        for (int prev = 0; prev < STATES * MASKS; ++prev) {
            seq_t ways = dpPrev[prev];
            if (!ways) continue;
            unsigned char cnt = nextCount[prev];
            for (unsigned char k = 0; k < cnt; ++k) {
                int next = nextList[prev][k];
                dpNext[next] += ways;
            }
        }
        /* swap */
        for (int i = 0; i < STATES * MASKS; ++i) dpPrev[i] = dpNext[i];
    }

    /* Sum states where no down requirement remains */
    seq_t ans = 0;
    for (int i = 0; i < STATES * MASKS; ++i)
        if ((i & 7) == 0) ans += dpPrev[i];
    return ans;
}
