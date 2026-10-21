/*
 * A211780  a(n) = Sum_{d|n, d<n} d * tau(n / d), where tau = A000005 is the
 *          number of divisors.
 * https://oeis.org/A211780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} d * tau(n/d), tau = number of divisors
 * Range:     u8 n <= 59, u16 n <= 6719, u32 n <= 9364, u64 n <= 9364,
 *            u128 n <= 9364, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute the divisor count (tau) of a 64‑bit integer */
static inline seq_t tau_uint64(idx_t n)
{
    seq_t cnt = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            do
            {
                tmp /= p;
                ++exp;
            } while (tmp % p == 0);
            cnt *= (exp + 1);
        }
    }
    if (tmp > 1)
        cnt *= 2;   /* remaining prime factor */
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;   /* a(1) = 0 */

    seq_t sum = 0;

    /* iterate over divisors up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 < n)
                sum += (seq_t)d1 * tau_uint64(d2);

            if (d2 != d1 && d2 < n)
                sum += (seq_t)d2 * tau_uint64(d1);
        }
    }
    return sum;
}
