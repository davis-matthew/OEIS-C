/*
 * A087452  G.f.: (2-x)/((1+3*x)*(1-4*x)); e.g.f.: exp(4*x) + exp(-3*x); a(n)
 *          = 4^n + (-3)^n.
 * https://oeis.org/A087452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + (-3)^n (a(0)=2, a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Binary exponentiation for an arbitrary‐precision unsigned integer type.
   base  – base value, 4 or 3.
   exp   – exponent (unsigned). */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;      /* identity element for multiplication   */
    seq_t b = base;        /* current power of base                */

    while (exp)
    {
        if (exp & 1)       /* if the current bit is set */
            result *= b;   /* multiply the accumulated result */

        b *= b;            /* square the base for next bit */
        exp >>= 1;         /* move to the next bit of exp     */
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 4^n */
    seq_t pow4 = pow_seq_t(4, n);

    /* 3^n */
    seq_t pow3 = pow_seq_t(3, n);

    /* (-3)^n = (-1)^n * 3^n.  If n is even, add; otherwise subtract. */
    if ((n & 1) == 0)
        return pow4 + pow3;   /* n even: 4^n + 3^n   */
    else
        return pow4 - pow3;   /* n odd : 4^n - 3^n   */
}
