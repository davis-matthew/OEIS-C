/*
 * A000669  Number of series-reduced planted trees with n leaves. Also the
 *          number of essentially series series-parallel networks with n
 *          edges; also the number of essentially parallel series-parallel
 *          networks with n edges.
 * https://oeis.org/A000669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Product_{k>0} 1/(1-x^k)^a_k = 1+x+2*Sum_{k>1} a_k*x^k, a(1) = 1.
 *            Taking the coefficient of x^n gives a(n) = [x^n] Product_{k<n}
 *            1/(1-x^k)^a(k). Equivalently, using the Euler transform with
 *            c(k) = Sum_{d|k} d*a(d): a(n) = ( Sum_{k=1..n-1} c(k)*b(n-k) +
 *            Sum_{d|n,d<n} d*a(d) ) / n, where b(0) = 1, b...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Helpers:   fixed_compute, grow_local, seq_is_wide, u_gcd_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/fixed_compute.h"
#include "../utils/grow_local.h"
#include "../utils/seq_is_wide.h"
#include "../utils/u_gcd_local.h"

static seq_t *A = NULL, *B = NULL, *C = NULL;
static idx_t have = 0, cap = 0;

static seq_t big_compute(idx_t n) {
    idx_t m, k, d;
    if (have == 0) {
        cap = 64;
        A = grow_local(NULL, 0, cap);
        B = grow_local(NULL, 0, cap);
        C = grow_local(NULL, 0, cap);
        A[1] = 1; B[0] = 1; B[1] = 1; C[1] = 1;
        have = 1;
    }
    if (n < cap && n <= have) return A[n];
    if (n >= cap) {
        idx_t nc = cap;
        while (nc <= n) nc *= 2;
        A = grow_local(A, cap, nc);
        B = grow_local(B, cap, nc);
        C = grow_local(C, cap, nc);
        cap = nc;
    }
    for (m = have + 1; m <= n; m++) {
        seq_t S = 0, D = 0;
        for (k = 1; k < m; k++) S += C[k] * B[m - k];
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t q = m / d;
                if (d < m) D += (seq_t)d * A[d];
                if (q != d && q < m) D += (seq_t)q * A[q];
            }
        }
        A[m] = (S + D) / (seq_t)m;
        B[m] = A[m] * 2;
        C[m] = D + (seq_t)m * A[m];
        have = m;
    }
    return A[n];
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (seq_is_wide()) return big_compute(n);
    /* beyond n = 200 the value overflows any fixed width of at most 128 bits */
    if (n > 200) return 0;
    return fixed_compute(n);
}
