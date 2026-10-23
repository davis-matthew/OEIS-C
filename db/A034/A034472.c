/*
 * A034472  a(n) = 3^n + 1.
 * https://oeis.org/A034472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    return seq_pow((seq_t)3, n) + 1;
}
