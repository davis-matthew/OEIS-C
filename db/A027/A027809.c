/*
 * A027809  a(n) = 143*(n+1)*binomial(n+4,13)/2.
 * https://oeis.org/A027809
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 143*(n+1)*binomial(n+4,13)/2
 * Range:     u8 none, u16 n <= 10, u32 n <= 20, u64 n <= 89, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a multiplicative implementation of the binomial coefficient
   and defers the final multiplication by 143 after an early
   division by 2 to keep intermediate values small. */

seq_t a(idx_t n)
{
    /* For the OEIS offset 9 all terms with n < 9 are 0. */
    if (n < 9) return 0;

    /* Compute binom = C(n+4, 13) multiplicatively. */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 13; ++i) {
        seq_t num = (seq_t)(n - 9 + i);   /* (n-9)+i ranges from n-8 .. n+4 */
        binom = (binom * num) / i;
    }

    /* Ready to form a = binom * (n+1) * 143 / 2.
       Divide by 2 early to prevent overflow. */
    seq_t t1 = binom;
    seq_t t2 = (seq_t)(n + 1);

    if ((t2 & 1) == 0)        /* t2 is even */
        t2 /= 2;
    else                      /* t1 must be even */
        t1 /= 2;

    /* Final product: (t1 * t2) * 143. */
    seq_t result = (t1 * t2) * 143;
    return result;
}
