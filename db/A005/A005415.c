/*
 * A005415  Number of simple tensors with n external gluons.
 * https://oeis.org/A005415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n-2} binomial(n-1, k) * ((2*n-2*k-4)!/(n-k-2)!)
 *            * a(k), a(0) = 1, a(1) = 0; where (2m-4)!/(m-2)! = 2^(m-2) *
 *            (2m-5)!! for m >= 3 and = 1 for m = 2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    static seq_t *A = NULL;    /* A[i] = a(i) for i < len */
    static seq_t *C = NULL;    /* C[m] = (2m-4)!/(m-2)! for 2 <= m < len */
    static seq_t *row = NULL;  /* row[k] = binomial(len-1, k) */
    static seq_t *st = NULL;   /* st[0] = 2^(m-2), st[1] = (2m-5)!! for last m */
    static idx_t len = 0, cap = 0;

    if (len == 0) {
        cap = 16;
        A = grow_zero(NULL, 0, cap);
        C = grow_zero(NULL, 0, cap);
        row = grow_zero(NULL, 0, cap);
        st = grow_zero(NULL, 0, 2);
        A[0] = 1;
        row[0] = 1;
        st[0] = 1;  /* 2^0 for m = 2 */
        st[1] = 1;  /* (-1)!! */
        len = 1;
    }

    if (n >= len) {
        if (n + 1 > cap) {
            idx_t nc = cap;
            while (nc < n + 1) nc *= 2;
            A = grow_zero(A, cap, nc);
            C = grow_zero(C, cap, nc);
            row = grow_zero(row, cap, nc);
            cap = nc;
        }
        for (idx_t i = len; i <= n; i++) {
            /* C[i] */
            if (i == 2) {
                C[i] = 1;
            } else if (i >= 3) {
                st[0] *= 2;
                st[1] *= (seq_t)(2 * i - 5);
                C[i] = st[0] * st[1];
            }
            /* a(i) using row = binomial(i-1, .) */
            seq_t s = 0;
            for (idx_t k = 0; k + 2 <= i; k++)
                s += row[k] * C[i - k] * A[k];
            A[i] = s;
            /* advance row to binomial(i, .) */
            row[i] = 1;
            for (idx_t k = i - 1; k >= 1; k--)
                row[k] += row[k - 1];
        }
        len = n + 1;
    }

    return A[n];
}
