/*
 * A007649  Number of set-like molecular species of degree n.
 * https://oeis.org/A007649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Euler transform of b, where b(1) = 1 and b(n) = Sum_{d|n,
 *            d<n} a(d) for n > 1 (A007650 = MOEBIUSi(c) - c). a(0) = 1, a(n)
 *            = (1/n) * Sum_{k=1..n} c(k)*a(n-k), c(k) = Sum_{d|k} d*b(d)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 43, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *A = NULL, *B = NULL, *C = NULL;
static idx_t done = 0, cap = 0;

static void ensure(idx_t n) {
    if (n < done) return;
    if (n + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        A = grow_zero(A, cap, nc);
        B = grow_zero(B, cap, nc);
        C = grow_zero(C, cap, nc);
        cap = nc;
    }
    if (done == 0) {
        A[0] = 1;
        done = 1;
    }
    for (idx_t i = done; i <= n; i++) {
        /* b(i) */
        seq_t bi = 0;
        if (i == 1) {
            bi = 1;
        } else {
            for (idx_t j = 1; j * j <= i; j++) {
                if (i % j == 0) {
                    idx_t q = i / j;
                    bi += A[j];
                    if (q != j && q != i) bi += A[q];
                }
            }
        }
        B[i] = bi;
        /* c(i) */
        seq_t ci = 0;
        for (idx_t j = 1; j * j <= i; j++) {
            if (i % j == 0) {
                idx_t q = i / j;
                ci += (seq_t)j * B[j];
                if (q != j) ci += (seq_t)q * B[q];
            }
        }
        C[i] = ci;
        /* a(i) */
        seq_t s = 0;
        for (idx_t k = 1; k <= i; k++) s += C[k] * A[i - k];
        A[i] = s / (seq_t)i;
        done = i + 1;
    }
}

seq_t a(idx_t n) {
    ensure(n);
    return A[n];
}
