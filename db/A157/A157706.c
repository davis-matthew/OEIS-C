/*
 * A157706  The z^2 coefficients of the polynomials in the GF1 denominators of
 *          A156921.
 * https://oeis.org/A157706
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (offset 2):  a(n) = 7*binom(n+4,6) + 26*binom(n+3,6) + 7*binom(n+2,6)
   for all n ≥ 2.  The binomial coefficient is computed multiplicatively to
   keep all intermediate values within the domain of seq_t. */

static seq_t binom6(seq_t m)
{
    /* Compute binomial(m,6) using the identity
       C(m,i) = C(m,i-1) * (m-i+1) / i, where C(m,0)=1.
       The division is exact at each step, so the result stays an integer. */
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= 6; ++i) {
        r = r * (m - (seq_t)(i - 1)) / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2) {
        /* behaviour for n < 2 is irrelevant; return 0 */
        return (seq_t)0;
    }

    /* Convert the index to seq_t before use. */
    seq_t m1 = (seq_t)(n + 4);
    seq_t m2 = (seq_t)(n + 3);
    seq_t m3 = (seq_t)(n + 2);

    seq_t c1 = binom6(m1);
    seq_t c2 = binom6(m2);
    seq_t c3 = binom6(m3);

    const seq_t c7  = (seq_t)7;
    const seq_t c26 = (seq_t)26;

    seq_t result = c1 * c7 + c2 * c26 + c3 * c7;
    return result;
}
