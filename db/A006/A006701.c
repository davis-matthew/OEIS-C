/*
 * A006701  Exponentiation of g.f. for Fibonacci numbers.
 * https://oeis.org/A006701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * A000045(k) * a(n-k-1), a(-1)
 *            = 1, so a(0) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *A = NULL;   /* A[i] = a(i) */
static seq_t *F = NULL;   /* F[i] = Fibonacci(i) */
static seq_t *R = NULL;   /* R[k] = binomial(len-1, k) */
static idx_t len = 0;     /* number of a-values computed */
static idx_t cap = 0;

static void reserve(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    A = grow_zero(A, cap, nc);
    F = grow_zero(F, cap, nc);
    R = grow_zero(R, cap, nc);
    cap = nc;
}

seq_t a(idx_t n) {
    idx_t i, k;
    if (n < len) return A[n];
    reserve(n + 1);
    for (i = len; i <= n; i++) {
        seq_t sum = 0;
        if (i == 0) F[i] = 0;
        else if (i == 1) F[i] = 1;
        else F[i] = F[i - 1] + F[i - 2];

        /* advance Pascal row to row i */
        if (i == 0) {
            R[0] = 1;
        } else {
            R[i] = 1;
            for (k = i - 1; k >= 1; k--) R[k] += R[k - 1];
        }

        for (k = 0; k <= i; k++) {
            if (k == i) sum += R[k] * F[k];
            else sum += R[k] * F[k] * A[i - k - 1];
        }
        A[i] = sum;
    }
    len = n + 1;
    return A[n];
}
