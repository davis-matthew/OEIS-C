/*
 * A083225  a(n) = (7*8^n + (-8)^n)/8.
 * https://oeis.org/A083225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (7*8^n + (-8)^n)/8 Simplification: a(2k) = 8^(2k) = 8^n
 *            a(2k+1) = 6*8^(2k) = 6*8^(n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility for exponentiation of seq_t */
#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n)
{
    /* Even index: a(n) = 8^n */
    if ( (n & 1u) == 0 )
        return _pow_seq_t((seq_t)8, n);

    /* Odd index: a(n) = 6 * 8^(n-1) */
    return (seq_t)6 * _pow_seq_t((seq_t)8, n - 1);
}
