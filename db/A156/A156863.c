/*
 * A156863  Denominator of Euler(n, 1/22).
 * https://oeis.org/A156863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 22^n if n even a(n) = 22^n / 2 if n odd
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the pre‑existing utility pow_uint() for arbitrary‑precision exponentiation. */
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t res = pow_uint(22, n);   /* compute 22^n */
    if (n & 1)                     /* if n is odd, divide by 2 */
        res /= 2;
    return res;
}
