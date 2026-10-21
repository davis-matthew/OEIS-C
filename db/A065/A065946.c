/*
 * A065946  Bessel polynomial {y_n}''(-2).
 * https://oeis.org/A065946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=2}^{n} (-1)^k * (n+k)!/(k!(n-k)!) * k(k-1) / 4
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    seq_t C   = (seq_t)1;           /* C_{n,0} = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k >= 2)
        {
            seq_t term = C * ((seq_t)k * (seq_t)(k - 1));
            if (k & 1u)                /* k odd ⇒ sign negative */
                term = -term;
            sum += term;
        }

        if (k < n)                     /* raise C to next k+1 */
        {
            seq_t mult = (seq_t)(n - k) * (seq_t)(n + k + 1);
            C = (C * mult) / (seq_t)(k + 1);
        }
    }

    return sum / (seq_t)4;
}
