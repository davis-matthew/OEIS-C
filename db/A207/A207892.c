/*
 * A207892  Number of nX5 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of n×5 0..1 arrays avoiding
   horizontally 0 0 0 and 1 0 1, vertically 0 0 0 and 1 1 1. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Precomputed data is built only once. */
    static bool built = false;
    /* 13 allowed row patterns (see OEIS a(1)). */
    static int pat[13];
    /* mapping pattern → index in pat array (-1 if not allowed). */
    static int patIdx[32];
    /* Transition lists: for each state (p0,p1) we store all possible p2 indices. */
    static int nextCnt[169];                 /* number of next patterns for each state */
    static int nextP[169][13];               /* allowed next pattern indices (max 13) */
    /* DP arrays for current and next step. */
    static seq_t cur[169];
    static seq_t nxt[169];

    if (!built) {
        /* Build allowed row patterns. */
        int pi = 0;
        for (int m = 0; m < 32; ++m) {
            bool ok = true;
            for (int i = 0; i <= 2; ++i) {
                int w = (m >> i) & 7;
                if (w == 0 || w == 5) { ok = false; break; }
            }
            if (ok) {
                pat[pi] = m;
                patIdx[m] = pi;
                ++pi;
            } else {
                patIdx[m] = -1;
            }
        }
        /* Pre‑compute transitions. */
        for (int p0 = 0; p0 < 13; ++p0) {
            int m0 = pat[p0];
            for (int p1 = 0; p1 < 13; ++p1) {
                int st = p0 * 13 + p1;
                int m1 = pat[p1];
                int cnt = 0;
                for (int p2 = 0; p2 < 13; ++p2) {
                    int m2 = pat[p2];
                    bool ok = true;
                    for (int c = 0; c < 5 && ok; ++c) {
                        int b0 = (m0 >> c) & 1;
                        int b1 = (m1 >> c) & 1;
                        int b2 = (m2 >> c) & 1;
                        if (b0 == b1 && b1 == b2) ok = false;
                    }
                    if (ok) {
                        nextP[st][cnt++] = p2;
                    }
                }
                nextCnt[st] = cnt;
            }
        }
        built = true;
    }

    /* base cases */
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)169;

    /* Initialise DP for two rows (all pairs are allowed). */
    for (int s = 0; s < 169; ++s) cur[s] = 1;
    /* perform n-2 transitions */
    for (idx_t step = 3; step <= n; ++step) {
        for (int s = 0; s < 169; ++s) nxt[s] = 0;
        for (int s = 0; s < 169; ++s) {
            if (cur[s] == 0) continue;
            int p1 = s % 13;
            int c = nextCnt[s];
            for (int k = 0; k < c; ++k) {
                int p2 = nextP[s][k];
                int t = p1 * 13 + p2;
                nxt[t] += cur[s];
            }
        }
        for (int s = 0; s < 169; ++s) cur[s] = nxt[s];
    }
    /* sum all states to get total number of arrays of size n. */
    seq_t total = 0;
    for (int s = 0; s < 169; ++s) total += cur[s];
    return total;
}
