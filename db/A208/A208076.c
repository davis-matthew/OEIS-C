/*
 * A208076  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 0 vertically.
 * https://oeis.org/A208076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over the 26 allowed 6‑bit rows with a transition table that
 * encodes the vertical constraints (no 000 or 010).  a(n) is the number of
 * n×6 binary arrays satisfying the given restrictions. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static data that is built once for all calls. */
    static bool initialized = false;
    static int rows[26];                 /* allowed 6‑bit patterns          */
    static int idx_of_val[64];           /* mapping from 0..63 to row index */
    static int aIdxArr[676];             /* first row of state (i,j)       */
    static int bIdxArr[676];             /* second row of state (i,j)      */
    static int nextCnt[676];             /* number of valid next rows      */
    static int nextC[676][26];           /* transition list for each state */

    if (!initialized) {
        /* Build the list of rows that satisfy horizontal constraints. */
        int cnt = 0;
        for (int r = 0; r < 64; ++r) {
            bool ok = true;
            for (int j = 0; j < 4 && ok; ++j) {
                int b0 = (r >> j) & 1;
                int b1 = (r >> (j + 1)) & 1;
                if (b0 == 0 && b1 == 0) ok = false;
            }
            if (ok) {
                rows[cnt] = r;
                idx_of_val[r] = cnt;
                ++cnt;
            }
        }

        /* Prepare state to row index mapping. */
        for (int i = 0; i < 26; ++i)
            for (int j = 0; j < 26; ++j) {
                int st = i * 26 + j;
                aIdxArr[st] = i;
                bIdxArr[st] = j;
            }

        /* Compute transition lists. */
        for (int i = 0; i < 26; ++i)
            for (int j = 0; j < 26; ++j) {
                int st = i * 26 + j;
                int a = rows[i];
                int b = rows[j];
                int ccount = 0;
                for (int k = 0; k < 26; ++k) {
                    int c = rows[k];
                    bool ok = true;
                    for (int col = 0; col < 6 && ok; ++col) {
                        int v0 = (a >> col) & 1;
                        int v1 = (b >> col) & 1;
                        int v2 = (c >> col) & 1;
                        if (v0 == 0 && v1 == 0 && v2 == 0) ok = false;
                        if (v0 == 0 && v1 == 1 && v2 == 0) ok = false;
                    }
                    if (ok) {
                        nextC[st][ccount++] = k;
                    }
                }
                nextCnt[st] = ccount;
            }
        initialized = true;
    }

    /* Handle small n directly (these are also produced by the DP). */
    if (n == 0) return (seq_t)0;   /* undefined by OEIS but harmless if called */
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)676;

    seq_t dpCurr[676];
    seq_t dpNext[676];

    /* Initialise DP for height 2: all pairs are valid. */
    for (int s = 0; s < 676; ++s) dpCurr[s] = (seq_t)1;

    /* Extend rows from height 3 up to n. */
    for (idx_t row = 3; row <= n; ++row) {
        for (int s = 0; s < 676; ++s) dpNext[s] = (seq_t)0;

        for (int s = 0; s < 676; ++s) {
            seq_t cur = dpCurr[s];
            if (cur == 0) continue;
            int bj = bIdxArr[s];
            int ccount = nextCnt[s];
            for (int t = 0; t < ccount; ++t) {
                int ci = nextC[s][t];
                int dest = bj * 26 + ci;
                dpNext[dest] += cur;
            }
        }

        /* swap dpCurr and dpNext by copying dpNext into dpCurr */
        for (int s = 0; s < 676; ++s) dpCurr[s] = dpNext[s];
    }

    /* Sum all states to get the total number of arrays of height n. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 676; ++s) result += dpCurr[s];
    return result;
}
