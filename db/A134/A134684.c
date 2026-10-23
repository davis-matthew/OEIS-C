/*
 * A134684  a(n) = n^(n+9).
 * https://oeis.org/A134684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+9)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* a(0) = 0 by definition */
    return big_pow((seq_t)n, (seq_t)(n + 9));
}
