/*
 * A194075  Natural interspersion of A194073; a rectangular array, by
 *          antidiagonals.
 * https://oeis.org/A194075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   for n ≥ 1 let (i , j) be the coordinates of the n‑th * element
 *            of the natural interspersion array when enumerated *
 *            antidiagonally. * a(n) = [1 + 3 * Σ_{k=1}^{j-1} ceil(k/2)] //
 *            row 1 value at column j * + 3 * ⌊(i-1)/3⌋ * ( j + ⌊(i-1)/3⌋ + 1
 *            ) * + (i-1) mod 3
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find diagonal d such that cumulative count up to d‑1 < n ≤ cumulative count up to d.
       cumulative count for first d diagonals is d*(d+1)/2.  */
    idx_t d = 1;
    idx_t cum = 1;                     /* sum of lengths of processed diagonals */
    while (cum < n) {
        ++d;
        cum += d;
    }
    idx_t cum_prev = cum - d;          /* sum up to diagonal d‑1 */
    idx_t pos = n - cum_prev;           /* 1‑based position inside diagonal d */

    /* Convert diagonal position to matrix coordinates (i , j),
       1‑based indices. */
    idx_t i = pos;
    idx_t j = d - pos + 1;

    /* ---- row 1 value at column j ---- */
    idx_t m = j - 1;                    /* number of steps from column 1 */
    /* sum_{k=1}^{m} ceil(k/2) = ceil(m/2) * ( floor(m/2) + 1 ) */
    idx_t a_part = (m + 1) / 2;          /* ceil(m/2)  */
    idx_t b_part = m / 2;                /* floor(m/2) */
    idx_t sumCeil = a_part * (b_part + 1);
    seq_t base = (seq_t)1 + (seq_t)3 * (seq_t)sumCeil;

    /* ---- offset contributed by higher rows ---- */
    idx_t k = (i - 1) / 3;               /* group number of the row */
    idx_t r = (i - 1) % 3;               /* position inside the group [0..2] */
    seq_t temp = (seq_t)(j + k + 1);
    seq_t offset = (seq_t)3 * (seq_t)k * temp + (seq_t)r;

    /* ----- final value ----- */
    return base + offset;
}
