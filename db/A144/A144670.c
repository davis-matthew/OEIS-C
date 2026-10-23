/*
 * A144670  Triangle read by rows where T(m,n)=2mn+m+n-7.
 * https://oeis.org/A144670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = T(m,k) read by rows, where T(m,k)=2*m*k+m+k-7 and m,k are
 *            1‑based row and column indices, respectively
 * Range:     i8 n <= 35, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The index n (1‑based) maps to (m,k) via the triangular numbers. */
#include <stdint.h>
#include "../utils/int_sqrt.h"

static inline idx_t triangular(idx_t m) {
    return (idx_t)(((__int128)m * (m + 1)) / 2);
}

seq_t a(idx_t n) {
    /* n is guaranteed >= 1 for the problem specification. */
    idx_t t = 8ull * n + 1ull;
    idx_t sqrt_t = int_sqrt(t);                 /* floor(sqrt(8n+1)) */
    idx_t m = (sqrt_t - 1) / 2;                 /* candidate row */
    /* Ensure triangular(m) >= n, adjust if necessary. */
    while (triangular(m) < n) ++m;
    while (m > 1 && triangular(m - 1) >= n) --m;
    idx_t tri_prev = triangular(m - 1);          /* total elements before row m */
    idx_t k = n - tri_prev;                      /* column index in row m (1‑based) */
    /* Compute the triangle value. */
    seq_t val = (seq_t)(2ull * m * k) + (seq_t)m + (seq_t)k - (seq_t)7;
    return val;
}
