/*
 * A005727  n-th derivative of x^x at x=1. Also called Lehmer-Comtet numbers.
 * https://oeis.org/A005727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = a(n-1) + Sum_{k=0..n-2}
 *            (-1)^(n-k)*(n-2-k)!*binomial(n-1,k)*a(k), a(0) = 1
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 35,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *A = NULL;   /* memoized terms a(0..cnt-1) */
static seq_t *F = NULL;   /* factorials F[j] = j! */
static seq_t *B = NULL;   /* binomial row cnt-1: B[k] = binomial(cnt-1, k) */
static idx_t cap = 0;
static idx_t cnt = 0;

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    if (cap == 0) {
        A = alloc_zero(nc);
        F = alloc_zero(nc);
        B = alloc_zero(nc);
    } else {
        A = grow_zero(A, cap, nc);
        F = grow_zero(F, cap, nc);
        B = grow_zero(B, cap, nc);
    }
    cap = nc;
}

seq_t a(idx_t n) {
    ensure_cap(n + 2);
    if (cnt == 0) {
        A[0] = 1;
        F[0] = 1;
        B[0] = 1;
        cnt = 1;
    }
    while (cnt <= n) {
        idx_t m = cnt;
        /* factorials up to m */
        F[m] = F[m - 1] * (seq_t)m;
        /* B currently holds row m-1 */
        seq_t s = A[m - 1];
        for (idx_t k = 0; k + 1 < m; k++) {
            seq_t t = F[m - 2 - k] * B[k] * A[k];
            if (((m - k) & 1) == 0) s += t;
            else s -= t;
        }
        A[m] = s;
        /* advance B to row m */
        B[m] = 0;
        for (idx_t k = m; k >= 1; k--) {
            B[k] += B[k - 1];
        }
        cnt = m + 1;
    }
    return A[n];
}
