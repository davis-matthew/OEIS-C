/*
 * A081131  a(n) = n^(n-2) * binomial(n,2).
 * https://oeis.org/A081131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(n-2) * binomial(n,2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the binomial coefficient is zero, so the term is 0. */
    if (n < 2) return 0;

    /* Compute binomial(n,2) = n(n-1)/2, using seq_t arithmetic. */
    seq_t binom = ((seq_t)n * ((seq_t)n - 1)) / 2;

    /* Compute n^(n-2) using exponentiation by squaring. */
    seq_t pow = 1;            /* accumulator for the result         */
    seq_t base = (seq_t)n;    /* current power of n                 */
    idx_t e = n - 2;          /* exponent for n^(n-2)                */

    while (e > 0) {
        if (e & 1) pow *= base;   /* multiply when the current bit is set */
        base *= base;             /* square the base for the next bit     */
        e >>= 1;                  /* shift exponent right by one          */
    }

    /* Multiply the power by the binomial coefficient to get the term. */
    return pow * binom;
}
