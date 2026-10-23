/*
 * A177098  a(n) = 6*(10^n-7).
 * https://oeis.org/A177098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(10^n - 7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t v = pow10(n);          /* 10^n */
    v = v * (seq_t)6 - (seq_t)42; /* 6*(10^n) - 42 */
    return v;
}
