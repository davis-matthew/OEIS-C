/*
 * A130505  a(n) = 3*a(n-1) if n is odd, otherwise 6*a(n-1).
 * https://oeis.org/A130505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18^(⌊n/2⌋) * (3 if n is odd else 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(n) = 3*a(n-1) if n is odd, otherwise 6*a(n-1). */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;                    /* floor(n/2) */
    seq_t val = pow_int((seq_t)18, k);   /* 18^k */
    if (n & 1) val = val * (seq_t)3;     /* multiply by 3 for odd n */
    return val;
}
