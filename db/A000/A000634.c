/*
 * A000634  Number of glycols with n carbon atoms.
 * https://oeis.org/A000634
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   path decomposition between the two OH carbons. R(x) = A000598
 *            g.f. (alkyl radicals), R(x) = 1 + x*(R(x)^3 + 3*R(x)*R(x^2) +
 *            2*R(x^3))/6; bead B(x) = x*(R(x)^2 + R(x^2))/2; T = 1/(1-B(x));
 *            V = 1/(1-B(x^2)); D(x) = (B(x)^2 - B(x^2))/2 (unordered pairs of
 *            distinct beads); a(n) = [x^n] ( V*D*T + (1+B...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   a634_half_prod, alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/a634_half_prod.h"

static seq_t *A634_memo = 0;
static idx_t A634_cap = 0;

static void a634_build(idx_t N)
{
    idx_t L = N + 1, i, j, k, s;
    seq_t *R = alloc_zero(L), *R2 = alloc_zero(L), *B = alloc_zero(L);
    seq_t *D = alloc_zero(L), *T = alloc_zero(L), *V = alloc_zero(L);
    seq_t *DT = alloc_zero(L), *A = alloc_zero(L);

    R[0] = 1;
    R2[0] = 1;
    for (i = 1; i <= N; i++) {
        idx_t m = i - 1;
        seq_t c3 = 0, c21 = 0, c111 = 0, t = 0;
        for (j = 0; j <= m; j++) c3 += R2[j] * R[m - j];
        for (j = 0; 2 * j <= m; j++) c21 += R[j] * R[m - 2 * j];
        if (m % 3 == 0) c111 = R[m / 3];
        R[i] = (c3 + 3 * c21 + 2 * c111) / 6;
        for (j = 0; j <= i; j++) t += R[j] * R[i - j];
        R2[i] = t;
    }

    /* beads: one carbon plus a multiset of two radicals */
    for (s = 1; s <= N; s++) {
        idx_t t = s - 1;
        seq_t m = 0;
        for (k = 0; 2 * k < t; k++) m += R[k] * R[t - k];
        if (t % 2 == 0) {
            seq_t x = R[t / 2];
            m += a634_half_prod(x, x + 1);
        }
        B[s] = m;
    }

    /* unordered pairs of distinct beads */
    for (s = 1; s <= N; s++) {
        seq_t m = 0;
        for (k = 1; 2 * k < s; k++) m += B[k] * B[s - k];
        if (s % 2 == 0) {
            seq_t x = B[s / 2];
            if (x > 0) m += a634_half_prod(x, x - 1);
        }
        D[s] = m;
    }

    T[0] = 1;
    for (s = 1; s <= N; s++) {
        seq_t m = 0;
        for (k = 1; k <= s; k++) m += B[k] * T[s - k];
        T[s] = m;
    }

    V[0] = 1;
    for (s = 1; s <= N; s++) {
        seq_t m = 0;
        for (k = 1; 2 * k <= s; k++) m += B[k] * V[s - 2 * k];
        V[s] = m;
    }

    for (s = 0; s <= N; s++) {
        seq_t m = 0;
        for (k = 0; k <= s; k++) m += D[k] * T[s - k];
        DT[s] = m;
    }

    for (s = 0; s <= N; s++) {
        seq_t m = 0;
        for (k = 0; k <= s; k++) m += V[k] * DT[s - k];
        /* palindromes of length >= 2: (1+B)*(V-1) */
        if (s >= 1) m += V[s];
        for (k = 1; k < s; k++) m += B[k] * V[s - k];
        A[s] = m;
    }

    A634_memo = A;
    A634_cap = N;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (A634_memo == 0 || n > A634_cap) {
        idx_t N = 2 * A634_cap;
        if (N < n) N = n;
        if (N < 32) N = 32;
        a634_build(N);
    }
    return A634_memo[n];
}
