/*
 * A100029  Bisection of A008472.
 * https://oeis.org/A100029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of distinct primes dividing 2*n-1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2*n-1 using seq_t arithmetic */
    seq_t m = (seq_t)n * 2 - 1;
    seq_t sum = 0;

    /* trial division by all primes <= sqrt(m) */
    for (seq_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            sum += p;          /* distinct prime contributes once */
            while (m % p == 0)
                m /= p;        /* strip all instances of this prime */
        }
    }

    /* if a prime factor larger than sqrt remains */
    if (m > 1)
        sum += m;

    return sum;
}
