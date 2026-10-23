/*
 * A120785  Denominators of partial sums of Catalan numbers scaled by powers
 *          of 1/16.
 * https://oeis.org/A120785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 2^(4*n - popcount(n+1) + 1) where popcount(x) is the
 *            number of 1-bits in the binary representation of x. The
 *            denominator of the partial sum Σ_{k=0}^n Catalan(k)/16^k is a
 *            power of two with exponent 4*n - popcount(n+1) + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Count 1-bits in a 64-bit unsigned integer. */
static unsigned int popcount_u64(idx_t x)
{
    unsigned int c = 0;
    while (x)
    {
        c += (unsigned int)(x & 1ULL);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Compute exponent e = 4*n - popcount(n+1) + 1. */
    idx_t pc = popcount_u64(n + (idx_t)1);

    /* Use seq_t for the intermediate arithmetic to avoid
       overflow when n is large.  */
    seq_t exp = (seq_t)4 * (seq_t)n;   /* 4*n  */
    exp = exp - (seq_t)pc + 1;         /* 4*n - pc + 1 */

    /* Convert exponent back to idx_t for pow2 (it accepts idx_t). */
    return pow2((idx_t)exp);
}
