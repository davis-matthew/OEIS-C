/*
 * A010765  a(n) = floor(n/2)^floor(n/3).
 * https://oeis.org/A010765
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2)^floor(n/3)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* behaviour for n < 2 does not matter */
    if (n < 2) return 0;
    /* floor division on unsigned types is just C integer division */
    seq_t base = (seq_t)(n / 2);
    idx_t   exp  = (idx_t)(n / 3);
    return seq_pow(base, exp);
}
