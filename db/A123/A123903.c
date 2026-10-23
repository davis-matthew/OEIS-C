/*
 * A123903  Total number of "Emperors" in all tournaments on n labeled nodes.
 * https://oeis.org/A123903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^((n-1)*(n-2)/2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Compute the exponent e = (n-1)*(n-2)/2 without overflow. */
    idx_t e;
    if ((n - 1) & 1)           /* (n-1) is odd */
        e = (n - 1) * ((n - 2) >> 1);
    else                       /* (n-1) is even */
        e = ((n - 1) >> 1) * (n - 2);

    /* 2^e as a seq_t value. */
    seq_t val = pow2(e);

    /* Multiply by n. */
    val *= (seq_t)n;

    return val;
}
