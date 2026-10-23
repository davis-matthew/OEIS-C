/*
 * A168495  a(n) = 3^floor(n^2/3).
 * https://oeis.org/A168495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^floor(n^2/3)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute floor(n^2/3) in seq_t to avoid overflow. */
    seq_t sq = (seq_t)n * (seq_t)n;   /* n^2, exact in seq_t */
    seq_t exp = sq / ((idx_t)3);      /* floor division by 3 */

    /* Return 3^exp.  big_pow works for any seq_t type, including big integers. */
    return big_pow((seq_t)3, exp);
}
