/*
 * A120978  a(n) = 2n+5^n-3^n.
 * https://oeis.org/A120978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 5^n - 3^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow5 = seq_pow((seq_t)5, n);
    seq_t pow3 = seq_pow((seq_t)3, n);
    seq_t twon = (seq_t)2 * (seq_t)n;
    return twon + pow5 - pow3;
}
