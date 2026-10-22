/*
 * A000822  Expansion of (sin^2 x + sin x) /cos 2x.
 * https://oeis.org/A000822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   sin^2 x / cos 2x = (sec 2x - 1)/2 and sin x / cos 2x = odd part
 *            of 1/(cos x - sin x), so a(0) = 0, a(2m) = 2^(2m-1)*E(2m)
 *            (Euler/secant numbers, A000364), a(2m+1) = A001586(2m+1)
 *            (Springer numbers). Both come from the derivative polynomials of
 *            sec: P_0 = 1, P_{n+1}(t) = (1+t^2)P_n'(t) + t*P_n(t),...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   a306_alloc0, a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_alloc0.h"
#include "../utils/a306_grow.h"

static seq_t *P = NULL;   /* coefficients of the current derivative polynomial */
static seq_t *A = NULL;   /* memoized terms */
static idx_t cap = 0;     /* capacity of A (P has cap + 2) */
static idx_t cnt = 0;     /* number of terms computed so far */

seq_t a(idx_t n) {
    if (n >= cap) {
        idx_t nc = cap ? cap * 2 : 64;
        while (nc <= n) nc *= 2;
        if (!P) {
            P = a306_alloc0(nc + 2);
            A = a306_alloc0(nc);
        } else {
            P = a306_grow(P, cap + 2, nc + 2);
            A = a306_grow(A, cap, nc);
        }
        cap = nc;
    }
    while (cnt <= n) {
        idx_t m = cnt;
        if (m == 0) {
            P[0] = 1;
            A[0] = 0;
        } else {
            /* P_{m-1} -> P_m in place: q_j = (j+1) p_{j+1} + j p_{j-1} */
            seq_t prev = 0;
            idx_t j;
            for (j = 0; j <= m; j++) {
                seq_t cur = P[j];
                seq_t nxt = P[j + 1];
                P[j] = nxt * (seq_t)(j + 1) + prev * (seq_t)j;
                prev = cur;
            }
            if (m % 2 == 1) {
                seq_t s = 0;
                for (j = 0; j <= m; j++) s += P[j];
                A[m] = s;
            } else {
                seq_t t = P[0];
                for (j = 1; j < m; j++) t *= 2;
                A[m] = t;
            }
        }
        cnt++;
    }
    return A[n];
}
