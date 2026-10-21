/*
 * A206344  a(n) = floor(n/2)^n.
 * https://oeis.org/A206344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2)^n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* behavior below the offset is irrelevant */
    seq_t base = (seq_t)(n / 2);  /* floor division of n by 2 */
    return seq_pow(base, n);
}
