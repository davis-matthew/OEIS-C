/*
 * A128802  a(n) = n*(n-1)*8^n.
 * https://oeis.org/A128802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*8^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2u) return 0;
    return (seq_t)n * (seq_t)(n - 1) * big_pow((seq_t)8, (seq_t)n);
}
