/*
 * A213573  Rectangular array: (row n) = b**c, where b(h) = 2^(h-1), c(h) =
 *          (n-1+h)^2, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained from the array T(n,k) given by * T(n,k) =
 *            2^k·(n² + 2n + 3) – (n + k + 2)² + 2·(n + k + 1) – 1. * The
 *            array is read by falling antidiagonals (north‑west corner
 *            first). * Thus a(1)=T(1,1), a(2)=T(2,1), a(3)=T(1,2), …
 * Range:     u8 n <= 15, u16 n <= 91, u32 n <= 435, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small helper: compute 2^exp as seq_t.  exp fits into idx_t. */
static seq_t pow2_seq(idx_t exp) {
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res += res;   /* multiply by 2 */
    return res;
}

seq_t a(idx_t n) {
    /* Offset: n≥1.  Below the offset return 0. */
    if (n == 0) return (seq_t)0;

    /* Find the antidiagonal (s) such that s*(s+1)/2 ≥ n. */
    idx_t s = 0;
    while ((s * (s + 1)) / 2 < n)
        ++s;

    /* Elements before this diagonal. */
    idx_t prev = ((s - 1) * s) / 2;
    idx_t pos  = n - prev;          /* 1‑based position on diagonal */

    /* Map to row and column indices (1‑based). */
    seq_t row = (seq_t)pos;
    seq_t col = (seq_t)(s - pos + 1);

    /* Evaluate the closed‑form for T(row, col). */
    seq_t p2   = pow2_seq((idx_t)col);           /* 2^col */
    seq_t r2   = row * row;                     /* row^2 */
    seq_t term1 = p2 * (r2 + 2 * row + (seq_t)3);

    seq_t rc2   = (row + col + (seq_t)2);        /* (row+col+2) */
    seq_t term2 = rc2 * rc2;                    /* squared */

    seq_t rc1   = (row + col + (seq_t)1);        /* (row+col+1) */
    seq_t term3 = 2 * rc1;                      /* double */

    return term1 - term2 + term3 - (seq_t)1;
}
