/*
 * A125650  Numerator of n*(n+3)/(4*(n+1)*(n+2)) = Sum_{k=1..n}
 *          1/(k*(k+1)*(k+2)).
 * https://oeis.org/A125650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = n*(n+3)/2^min(3, valuation(n*(n+3),2))
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 by definition */
        return (seq_t)0;

    /* compute the numerator n*(n+3) in the full precision type */
    seq_t num = (seq_t)n * ((seq_t)n + (seq_t)3);

    /* Reduce by factors of 2, but no more than 3 times */
    seq_t k = 0;
    while (k < 3 && (num & (seq_t)1) == (seq_t)0) {
        num /= (seq_t)2;
        ++k;
    }

    return num;
}
