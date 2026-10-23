/*
 * A106194  Triangle read by rows, generated from binomial transforms of odd
 *          numbers.
 * https://oeis.org/A106194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{k=c}^{r} binom(r,k)*(2(k-c)+1), where r = row_of(n)
 *            (row index), c = n - r(r+1)/2 (column index)
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_of.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine the row r and column c containing index n */
    idx_t r = row_of(n);
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);   /* r*(r+1)/2 */
    idx_t c = n - offset;

    /* Compute the binomial‑transform value for this (r,c) */
    seq_t result = 0;
    for (idx_t k = c; k <= r; ++k) {
        seq_t bin = binom(r, k);                      /* binomial(r,k)  */
        seq_t coeff = ((seq_t)(k - c) * 2) + 1;        /* 2(k-c)+1      */
        result += bin * coeff;
    }
    return result;
}
