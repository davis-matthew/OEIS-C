/*
 * A191448  Dispersion of the odd integers greater than 1, by antidiagonals.
 * https://oeis.org/A191448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (t_row + 1) * 2^(col-1) - 1 where: t_row = 1 if row == 1
 *            t_row = 2*(row-1) otherwise row = position of n within its
 *            diagonal col = diagonal number - row + 1 diagonal number d =
 *            smallest integer with d*(d+1)/2 >= n
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;          /* find diagonal number */
    idx_t prev = (d - 1) * d / 2;             /* cumulative count up to previous diagonal */
    idx_t pos = n - prev;                     /* position inside current diagonal (1‑based) */
    idx_t row = pos;                          /* row index (1‑based) */
    idx_t col = d - pos + 1;                  /* column index (1‑based) */

    seq_t t_row;
    if (row == 1)
        t_row = 1;
    else
        t_row = ((seq_t)2) * (row - 1);       /* t(row) for odd integers >1 complement */

    seq_t pow2 = (seq_t)1 << (col - 1);       /* 2^(col-1) */
    seq_t result = (t_row + 1) * pow2 - 1;    /* a(n) */
    return result;
}
