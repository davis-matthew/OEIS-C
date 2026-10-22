/*
 * A003134  Orders of Weyl groups of type E_n.
 * https://oeis.org/A003134
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, fini, full, bref
 * Formula:   a(n) = |W(E_n)| = Product_{i=1..n} (m_i + 1), where m_i are the
 *            exponents of E_n. The exponents come from the positive roots,
 *            which are generated from the Cartan matrix: #{i : m_i >= k} =
 *            number of positive roots of height k. Finite sequence: n = 6, 7,
 *            8
 * Range:     u8 none, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   e_find
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/e_find.h"

seq_t a(idx_t n) {
    int A[8][8];
    int R[256][8];
    int H[256];
    int cnt = 0, start, i, j, k, t, r, maxh;
    int byh[64];
    seq_t res;

    if (n < 3 || n > 8) return 0;
    r = (int)n;

    memset(A, 0, sizeof(A));
    for (i = 0; i < r; i++) A[i][i] = 2;
    /* E_n Dynkin diagram (Bourbaki labels shifted to 0-based): 0-2-3-4-...-(n-1), with 1 attached to 3 */
    A[0][2] = A[2][0] = -1;
    for (k = 2; k + 1 < r; k++) A[k][k + 1] = A[k + 1][k] = -1;
    A[1][3] = A[3][1] = -1;

    for (i = 0; i < r; i++) {
        for (j = 0; j < 8; j++) R[cnt][j] = 0;
        R[cnt][i] = 1;
        H[cnt] = 1;
        cnt++;
    }

    start = 0;
    while (start < cnt) {
        int end = cnt;
        for (t = start; t < end; t++) {
            for (i = 0; i < r; i++) {
                int c = 0, p = 0, v[8];
                for (j = 0; j < r; j++) c += R[t][j] * A[j][i];
                /* simply laced: root strings have length at most 2, so p is 0 or 1 */
                if (R[t][i] > 0) {
                    for (j = 0; j < 8; j++) v[j] = R[t][j];
                    v[i]--;
                    if (e_find(R, cnt, v, r)) p = 1;
                }
                if (p - c > 0) {
                    for (j = 0; j < 8; j++) v[j] = R[t][j];
                    v[i]++;
                    if (!e_find(R, cnt, v, r) && cnt < 256) {
                        for (j = 0; j < 8; j++) R[cnt][j] = v[j];
                        H[cnt] = H[t] + 1;
                        cnt++;
                    }
                }
            }
        }
        start = end;
    }

    for (k = 0; k < 64; k++) byh[k] = 0;
    maxh = 0;
    for (t = 0; t < cnt; t++) {
        byh[H[t]]++;
        if (H[t] > maxh) maxh = H[t];
    }

    res = 1;
    for (k = 1; k <= maxh; k++) {
        int e = byh[k] - byh[k + 1]; /* number of exponents equal to k */
        for (j = 0; j < e; j++) res *= (seq_t)(k + 1);
    }
    return res;
}
