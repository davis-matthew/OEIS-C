/*
 * A134875  a(n)=the smallest sum of two nontrivial divisors of n, if any,
 *          whose product equals n; otherwise, a(n)=n.
 * https://oeis.org/A134875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest sum of two nontrivial divisors of n; if no
 *            such pair, a(n)=n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 */

    seq_t minSum = (seq_t)n;      /* start with n (no nontrivial pair found yet) */

    for (idx_t d = 2; d <= n / d; ++d)   /* iterate proper divisors up to sqrt(n) */
    {
        if (n % d == 0)
        {
            seq_t sum = (seq_t)d + (seq_t)(n / d);
            if (sum < minSum)
                minSum = sum;
        }
    }

    return minSum;                 /* if no divisor found, minSum remains n */
}
