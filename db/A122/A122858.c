/*
 * A122858  Expansion of E(k) * K(k) * (2/Pi)^2 in powers of q^2 where E(),
 *          K() are complete elliptic integrals and the nome q = exp( -Pi *
 *          K(k') / K(k)).
 * https://oeis.org/A122858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 8 * Σ_{d|n} (-1)^{d-1} d, with a(0) = 1
 * Range:     i8 n <= 11, i16 n <= 1439, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* contribution of divisor d1 */
            if (d1 & 1)   /* odd */
                sum += (seq_t)d1;
            else          /* even */
                sum -= (seq_t)d1;

            /* if distinct, contribution of divisor d2 */
            if (d2 != d1)
            {
                if (d2 & 1)
                    sum += (seq_t)d2;
                else
                    sum -= (seq_t)d2;
            }
        }
    }

    return sum * ((seq_t)8);
}
