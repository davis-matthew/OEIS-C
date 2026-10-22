/*
 * A002212  Number of restricted hexagonal polyominoes with n cells.
 * https://oeis.org/A002212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{j=0..n-1} Sum_{i=0..j} a(i)*a(j-i), a(0) = 1
 *            (fixed-width builds, no overflowing intermediates); a(n) =
 *            (3(2n-1)*a(n-1) - 5(n-2)*a(n-2))/(n+1), a(0) = 1, a(1) = 1
 *            (arbitrary-precision build)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   a2212_fixed, a2212_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a2212_fixed.h"
#include "../utils/a2212_grow.h"

static seq_t *a2212_A = NULL;   /* a(i) */
static seq_t *a2212_C = NULL;   /* c(i) = [x^i] A(x)^2, fixed-width only */
static idx_t a2212_len = 0;     /* number of computed terms */
static idx_t a2212_cap = 0;
static int a2212_mode = -1;     /* 1 = fixed width, 0 = big */

seq_t a(idx_t n) {
    if (n < 2) return 1;
    if (a2212_mode < 0) a2212_mode = a2212_fixed();

    if (n >= a2212_cap) {
        idx_t nc = 2 * a2212_cap;
        if (nc < n + 1) nc = n + 1;
        if (nc < 32) nc = 32;
        a2212_A = a2212_grow(a2212_A, a2212_len, nc);
        a2212_C = a2212_grow(a2212_C, a2212_len, nc);
        a2212_cap = nc;
    }

    while (a2212_len <= n) {
        idx_t i = a2212_len;
        if (i < 2) {
            a2212_A[i] = 1;
        } else if (a2212_mode) {
            a2212_A[i] = a2212_A[i - 1] + a2212_C[i - 1];
        } else {
            seq_t t1 = (seq_t)3 * ((seq_t)2 * (seq_t)i - 1) * a2212_A[i - 1];
            seq_t t2 = (seq_t)5 * ((seq_t)i - 2) * a2212_A[i - 2];
            a2212_A[i] = (t1 - t2) / ((seq_t)i + 1);
        }
        if (a2212_mode) {
            seq_t s = 0;
            idx_t k;
            for (k = 0; k <= i; k++) s += a2212_A[k] * a2212_A[i - k];
            a2212_C[i] = s;
        }
        a2212_len++;
    }
    return a2212_A[n];
}
