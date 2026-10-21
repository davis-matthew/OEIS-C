/*
 * A094015  Expansion of (1+4*x)/(1-8*x^2).
 * https://oeis.org/A094015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^{floor(n/2)} (n even) a(n) = 4·8^{floor(n/2)} (n odd)
 *            This follows directly from expanding (1+4x)/(1-8x²) as
 *            (1+4x)·∑_{k≥0}(8x²)ᵏ
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
#include <stdint.h>

static seq_t pow8(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        r = r * (seq_t)8;
    return r;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0)              /* even index */
        return pow8(n >> 1);       /*  n/2  */
    else                           /* odd index  */
        return pow8((n >> 1)) * (seq_t)4;   /* 4·8^{(n-1)/2} */
}
