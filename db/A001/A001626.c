/*
 * A001626  Number of 3-line Latin rectangles.
 * https://oeis.org/A001626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(n) = A000186(n) + 2*(n-1)*a(n-1), n > 1, where
 *            A000186(n) = Sum_{k=0..floor(n/2)}
 *            binomial(n,k)*D(n-k)*D(k)*U(n-2k) (Riordan), D = derangement
 *            numbers A000166: D(0)=1, D(1)=0, D(n) = (n-1)*(D(n-1)+D(n-2)), U
 *            = menage numbers A000179: U(0)=1, U(1)=-1, U(2)=0, U(3)=1,
 *            U(4)=2, U(5)=13, U...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   a001028_grow, a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001028_grow.h"
#include "../utils/a001028_z.h"

static seq_t *A = NULL, *D = NULL, *U = NULL;
static idx_t cnt = 0; /* indices 0..cnt-1 computed */
static idx_t cap = 0;

static void extend(idx_t n) {
    idx_t m, k;
    if (n < cnt) return;
    if (n + 1 > cap) {
        idx_t newcap = 2 * (n + 1);
        if (newcap < 16) newcap = 16;
        if (cap == 0) {
            A = a001028_z(newcap);
            D = a001028_z(newcap);
            U = a001028_z(newcap);
        } else {
            A = a001028_grow(A, cap, newcap);
            D = a001028_grow(D, cap, newcap);
            U = a001028_grow(U, cap, newcap);
        }
        cap = newcap;
    }
    for (m = cnt; m <= n; m++) {
        /* derangements */
        if (m == 0) D[m] = 1;
        else if (m == 1) D[m] = 0;
        else D[m] = (seq_t)(m - 1) * (D[m - 1] + D[m - 2]);
        /* menage numbers (U(1) = -1 is handled separately; stored as 0) */
        if (m == 0) U[m] = 1;
        else if (m == 1) U[m] = 0;
        else if (m == 2) U[m] = 0;
        else if (m == 3) U[m] = 1;
        else if (m == 4) U[m] = 2;
        else if (m == 5) U[m] = 13;
        else {
            seq_t t = (seq_t)m * U[m - 1] + (seq_t)2 * U[m - 2];
            t -= (seq_t)(m - 4) * U[m - 3];
            t -= U[m - 4];
            U[m] = t;
        }
        if (m <= 1) {
            A[m] = 0;
            continue;
        }
        {
            seq_t pos = 0, neg = 0, c = 1, K;
            for (k = 0; 2 * k <= m; k++) {
                idx_t r = m - 2 * k;
                seq_t term = c * D[m - k] * D[k];
                if (r == 1) neg += term;
                else pos += term * U[r];
                c = c * (seq_t)(m - k) / (seq_t)(k + 1);
            }
            K = pos - neg;
            A[m] = K + (seq_t)2 * (seq_t)(m - 1) * A[m - 1];
        }
    }
    cnt = n + 1;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    extend(n);
    return A[n];
}
