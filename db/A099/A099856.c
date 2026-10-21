/*
 * A099856  Expansion of (1+3*x)/(1-3*x).
 * https://oeis.org/A099856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^n - 0^n (note: 0^0 is taken to be 1, so a(0)=1; for
 *            n>0, 0^n=0, hence a(n) = 2*3^n for n>0)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3^n using exponentiation by squaring. */
    seq_t pow3   = (seq_t)1;          /* accumulator for the result */
    seq_t base   = (seq_t)3;          /* current power of 3 */
    idx_t  exp   = n;                 /* exponent remaining */

    while (exp > 0) {
        if (exp & 1)                /* if the lowest bit is set */
            pow3 *= base;

        exp >>= 1;                  /* shift exponent right */
        if (exp)                     /* avoid one extra squaring when done */
            base *= base;
    }

    /* Now pow3 == 3^n.  The formula is 2*3^n - 0^n. */
    seq_t result = pow3 * (seq_t)2;

    if (n == 0)                     /* subtract the 0^0 term */
        result -= (seq_t)1;

    return result;
}
