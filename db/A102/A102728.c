/*
 * A102728  Array read by antidiagonals: T(n, k) = ((n+1)^k-(n-1)^k)/2.
 * https://oeis.org/A102728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((row+1)^col − (row−1)^col) / 2, where row = d − offset,
 *            col = offset, d = ⌊(√(8·n+1)−1)/2⌋, offset = n − d(d+1)/2. The
 *            sequence is read by antidiagonals of the array
 *            T(r,c)=((r+1)^c−(r−1)^c)/2
 * Range:     u8 n <= 31, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* Find diagonal d */
    idx_t d = (idx_t)((sqrt((long double)(8 * n + 1)) - 1) / 2);
    /* Correct possible rounding errors */
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= n)
        ++d;
    while ((unsigned __int128)d * (d + 1) / 2 > n)
        --d;

    idx_t sum_d = d * (d + 1) / 2;
    idx_t offset = n - sum_d;

    idx_t row = d - offset;
    idx_t col = offset;

    if (row == 0) {                 /* Special handling for (row‑1) = -1 */
        if (col == 0)
            return 0;                /* ((1^0 - (-1)^0)/2) = 0 */
        return (col % 2 == 0) ? 0 : 1;  /* ((1^c - (-1)^c)/2) */
    }

    seq_t base_plus  = (seq_t)(row + 1);
    seq_t base_minus = (seq_t)(row - 1);

    seq_t pow_plus  = pow_seq(base_plus,  col);
    seq_t pow_minus = pow_seq(base_minus, col);

    return (pow_plus - pow_minus) / 2;
}
