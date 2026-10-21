/*
 * A072899  Denominator of c(n) where c(0)=1 c(n+1) = n/c(n) + 1.
 * https://oeis.org/A072899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of c(n) where * c(0) = 1, * c(n+1) = n / c(n)
 *            + 1. * The function returns a(n) for any n >= 0
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b)
{
    /* Euclidean algorithm for greatest common divisor. */
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;  /* c(0) = 1, so denominator is 1 */

    /* Keep c(n) as a reduced fraction p/q. */
    seq_t p = (seq_t)1; /* numerator of c(n) */
    seq_t q = (seq_t)1; /* denominator of c(n) */

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* c(i) = ((i-1)/c(i-1)) + 1 =
         *     p_i = (i-1)*q + p
         *     q_i = p
         */
        seq_t mul   = (seq_t)(i - (idx_t)1) * q;
        seq_t new_p = mul + p;
        seq_t new_q = p;

        /* Reduce the fraction to lowest terms. */
        seq_t g = gcd_seq(new_p, new_q);
        if (g != (seq_t)1)
        {
            new_p /= g;
            new_q /= g;
        }

        p = new_p;
        q = new_q;
    }

    return q; /* denominator of c(n) */
}
