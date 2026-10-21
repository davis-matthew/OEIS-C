/*
 * A078632  Number of geometric subsequences of [1,...,n] with integral
 *          successive-term ratio and length > 1.
 * https://oeis.org/A078632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{r > 1, j > 0} floor(n / r^j)
 * Range:     u8 n <= 63, u16 n <= 7261, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = (seq_t)0;   /* accumulated result */
    seq_t N     = (seq_t)n;   /* use seq_t for all computations */

    /* for each integer ratio r > 1 */
    for (idx_t r = 2; r <= n; ++r)
    {
        seq_t power = (seq_t)r;          /* r^1   */
        /* add floor(N / r^j) for all j such that r^j <= N */
        while (power <= N)
        {
            sum += N / power;

            /* avoid overflow of power *= r when power is close to N */
            if (power > N / (seq_t)r)
                break;
            power *= (seq_t)r;
        }
    }
    return sum;
}
