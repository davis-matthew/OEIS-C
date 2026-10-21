/*
 * A069972  a(n) = Sum_{d|2*n,d+1|2*n} d.
 * https://oeis.org/A069972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|2*n, d+1|2*n} d
 * Range:     u8 n <= 586, u16 n <= 9373, u32 n <= 9373, u64 n <= 9373,
 *            u128 n <= 9373, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2*n as a seq_t to avoid overflow in seq_t arithmetic */
    seq_t M = (seq_t)n * 2u;
    seq_t sum = 0;

    /* iterate over possible divisors up to sqrt(M) */
    for (idx_t i = 1; (seq_t)i * (seq_t)i <= M; ++i)
    {
        if (M % (seq_t)i == 0)          /* i is a divisor */
        {
            seq_t d1 = (seq_t)i;
            seq_t d2 = M / d1;

            /* check condition for d1 */
            if ((M % (d1 + 1)) == 0)
                sum += d1;

            /* if d2 is distinct, check condition */
            if (d2 != d1 && (M % (d2 + 1)) == 0)
                sum += d2;
        }
    }

    return sum;
}
