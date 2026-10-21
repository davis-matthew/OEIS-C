/*
 * A076015  Sum of the (n-1)-th powers of the first n integers.
 * https://oeis.org/A076015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n} m^(n-1), n >= 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over base values m = 1 .. n */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t term = 1;                 /* term = m^(n-1) */
        idx_t exp  = n - 1;              /* exponent (n-1) */
        seq_t base = (seq_t)m;           /* current base for binary exponentiation */

        while (exp) {
            if (exp & 1U) term *= base;  /* multiply when bit is set */
            exp >>= 1;                   /* shift exponent */
            if (exp) base *= base;       /* square base for next bit */
        }

        sum += term;                     /* add the term to the sum */
    }
    return sum;
}
