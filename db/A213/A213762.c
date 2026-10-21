/*
 * A213762  Rectangular array: (row n) = b**c, where b(h) = 2^(h-1), c(h) =
 *          2*n-3+2*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) where * T(i,j) = 2^j·(2·i+1) – 2·i – 2·j – 1 *
 *            (i,j) are obtained from the n‑th entry of the array * read by
 *            falling antidiagonals: let d satisfy d·(d+1)/2 ≥ n, * let prev =
 *            d·(d−1)/2, let offset = n−prev−1 (0‑based), * then i = offset+1
 *            and j = d−offset
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 465, u64 n <= 1953,
 *            u128 n <= 3240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t pow_two(idx_t exp) {
    seq_t result = 1;
    seq_t base = 2;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* safety for off‑by‑one callers */

    /* find the antidiagonal d (1‑based) such that d·(d+1)/2 ≥ n */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) d++;

    idx_t prev   = (d * (d - 1)) / 2;       /* terms before diag d */
    idx_t offset = n - prev - 1;            /* zero‑based index in diag d */
    idx_t i = offset + 1;                   /* row index  (≥1) */
    idx_t j = d - offset;                   /* column index (≥1) */

    /* compute 2^j */
    seq_t p2j = pow_two(j);

    /* compute 2*i+1 and 2*i+2*j+1 as seq_t to avoid overflow on idx_t */
    seq_t two_i  = (seq_t)2 * (seq_t)i;
    seq_t num1   = two_i + 1;                 /* 2*i+1 */
    seq_t num2   = two_i + (seq_t)2 * (seq_t)j + 1; /* 2*i+2*j+1 */

    seq_t val = p2j * num1 - num2;
    return val;
}
