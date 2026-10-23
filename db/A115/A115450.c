/*
 * A115450  Number triangle (1/((1-x)(1-2x)),-x)-(x/((1-x)(1-2x)),-x^2)
 *          (expressed in the notation of Riordan arrays).
 * https://oeis.org/A115450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   row_of, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115450 – Number triangle (1/((1-x)(1-2x)),-x)-(x/((1-x)(1-2x)),-x^2) */
/* a(n) = T(r,k);  r = row_of(n), k = n - r(r+1)/2                     */
/* T(n,k) = (2^(n-k+1)-1 - (2^(n-2k)-1) * (k <= n/2)) * (-1)^k        */

#include "../utils/row_of.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Determine the row r and position k within that row. */
    idx_t r = row_of(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Compute the first part: 2^(r-k+1) - 1. */
    seq_t part1 = seq_pow((seq_t)2, (idx_t)(r - k + 1)) - (seq_t)1;

    /* Compute the second part if k <= floor(r/2). */
    seq_t part2 = 0;
    if (k <= r / 2)
    {
        part2 = seq_pow((seq_t)2, (idx_t)(r - 2 * k)) - (seq_t)1;
    }

    /* Combine parts and apply the sign (-1)^k. */
    seq_t val = part1 - part2;
    if (k & 1)
        val = -val;

    return val;
}
