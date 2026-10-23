/*
 * A102345  a(n) = 3^n + (-1)^n.
 * https://oeis.org/A102345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + (-1)^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow3 = seq_pow((seq_t)3, n);
    /* For even n, add 1; for odd n, subtract 1 */
    if ((n & 1) == 0)
        return pow3 + (seq_t)1;
    else
        return pow3 - (seq_t)1;
}
