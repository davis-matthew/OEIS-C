/*
 * A039653  a(0) = 0; for n > 0, a(n) = sigma(n)-1.
 * https://oeis.org/A039653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = sigma(n) - 1, where sigma(n) is the
 *            sum of all positive divisors of n
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) defined as 0 */
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = 0;
    /* compute floor(sqrt(n)) without floating point */
    idx_t lim = 0;
    while ((lim + 1) <= n / (lim + 1))
        lim++;

    /* iterate over divisors up to the square root */
    for (idx_t i = 1; i <= lim; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            if (i == j) {
                sum += (seq_t)i;                 /* perfect square case */
            } else {
                sum += (seq_t)i + (seq_t)j;       /* pair of divisors */
            }
        }
    }

    /* subtract 1 to exclude the divisor n itself */
    sum -= (seq_t)1;
    return sum;
}
