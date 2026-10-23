/*
 * A016197  a(n) = 12^n - 11^n.
 * https://oeis.org/A016197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 12^n - 11^n
 * The function uses the helper big_pow which performs exponentiation by
 * squaring on arbitrary‑precision or native integer types.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow12 = big_pow((seq_t)12, n);
    seq_t pow11 = big_pow((seq_t)11, n);
    return pow12 - pow11;
}
