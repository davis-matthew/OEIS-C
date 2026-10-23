/*
 * A170737  Expansion of g.f.: (1+x)/(1-17*x).
 * https://oeis.org/A170737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n>0, a(n) = 18 * 17^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A170737 (offset 0). */
seq_t a(idx_t n)
{
    /* a(0) is the only special case.  For n>=1 we compute
       18 * 17^(n-1) using exponentiation by squaring. */
    if (n == 0u)
        return (seq_t)1;

    idx_t exp = n - 1u;          /* exponent (n-1) never underflows */
    seq_t base = (seq_t)17;      /* base 17 */
    seq_t res  = (seq_t)18;      /* multiply by 18 at the end */

    while (exp != 0u) {
        if (exp & 1u)
            res *= base;          /* multiply res by current base */
        base *= base;              /* square the base */
        exp >>= 1u;                /* shift exponent right */
    }
    return res;
}
