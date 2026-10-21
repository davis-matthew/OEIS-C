/*
 * A213752  Rectangular array: (row n) = b**c, where b(h) = 2*h-1, c(h) =
 *          b(n-1+h), n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A213752:  a(n) is read by falling antidiagonals of the array
 * where T(n,k) = k^2*(2*n+2*k-1) – (k*(k+1)*(4*k-1))/3. */

#include <stdint.h>

/* Find the diagonal index S such that n ≤ S*(S‑1)/2.  */
static idx_t diag_from_p(idx_t p)
{
    idx_t lo = 1, hi = 2;
    while ((__int128)hi * (hi - 1) / 2 < p) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * (mid - 1) / 2 >= p)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Compute T(row,col) by the closed‑form formula. */
static seq_t T_val(idx_t row, idx_t col)
{
    seq_t r = (seq_t)row;
    seq_t c = (seq_t)col;
    seq_t term1 = c * c * ((seq_t)2 * r + (seq_t)2 * c - (seq_t)1);
    seq_t term2 = c * (c + (seq_t)1) * ((seq_t)4 * c - (seq_t)1);
    return term1 - term2 / 3;
}

/* Public routine for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1 – undefined below */
    idx_t s = diag_from_p(n);            /* row + col  */
    idx_t prev = (idx_t)((__int128)(s - 1) * (s - 2) / 2);
    idx_t offset = n - prev - 1;          /* zero‑based offset in this diagonal */
    idx_t row = offset + 1;
    idx_t col = s - row;
    return T_val(row, col);
}
