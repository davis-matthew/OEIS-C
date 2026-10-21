/*
 * A202873  Symmetric matrix based on (1,3,7,15,31,...), by antidiagonals.
 * https://oeis.org/A202873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ∑_{k=1}^{min(i,j)} (2^{i-k+1}-1)(2^{j-k+1}-1), where
 *            (i,j) are the matrix coordinates of the n‑th entry enumerated
 *            along antidiagonals, starting from (1,1)
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Integer square root: floor(sqrt(x)) for unsigned 64-bit x. */
static inline idx_t isqrt_u64(idx_t x) {
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* highest power of four ≤ x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Determine the antidiagonal number d (size) that contains index n. */
static inline idx_t diagonal_of(idx_t n) {
    idx_t m = 8 * n + 1;                /* 8n+1 fits in 64‑bit for n ≤ 10⁴ */
    idx_t sqrt_m = isqrt_u64(m);
    idx_t d = (sqrt_m - 1) >> 1;        /* floor((sqrt(8n+1)-1)/2) */
    if (n > d * (d + 1) / 2) d++;       /* adjust if on the next diagonal */
    return d;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for offset 1, sentinel */

    /* Map linear index n to matrix coordinates (i,j). */
    idx_t d   = diagonal_of(n);        /* diagonal size (and number)   */
    idx_t prev = (d - 1) * d / 2;      /* elements before this diagonal */
    idx_t t     = n - prev - 1;        /* 0‑based offset on diagonal   */
    idx_t i     = t + 1;               /* row index   (1‑based)        */
    idx_t j     = d - t;               /* column index(1‑based)        */

    /* Compute M[i][j] = Σ_{k=1}^{min(i,j)} S[i-k+1] * S[j-k+1],
       where S[m] = 2^m − 1. */
    seq_t sum  = 0;
    idx_t limit = (i < j) ? i : j;     /* min(i,j)                     */
    for (idx_t k = 1; k <= limit; ++k) {
        idx_t r = i - k + 1;
        idx_t c = j - k + 1;
        seq_t si = pow2(r) - 1;        /* 2^r − 1                      */
        seq_t sj = pow2(c) - 1;        /* 2^c − 1                      */
        sum += si * sj;
    }
    return sum;
}
