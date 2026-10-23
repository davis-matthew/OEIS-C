/*
 * A170769  Expansion of g.f.: (1+x)/(1-49*x).
 * https://oeis.org/A170769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 50 * 49^(n-1) for n>0, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)           // a(0) = 1
        return (seq_t)1;

    /* compute 49^(n-1) by exponentiation by squaring */
    idx_t exp = n - 1;
    seq_t pow = (seq_t)1;      /* holds the accumulating result */
    seq_t base = (seq_t)49;    /* current power of 49 being squared */

    while (exp > 0) {
        if (exp & (idx_t)1)    /* odd exponent: include current base */
            pow *= base;
        base *= base;          /* square the base for next bit */
        exp >>= 1;             /* shift exponent right */
    }

    pow *= (seq_t)50;          /* multiply by 50 to get a(n) */
    return pow;
}
