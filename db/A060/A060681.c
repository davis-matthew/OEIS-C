/*
 * A060681  Largest difference between consecutive divisors of n (ordered by
 *          size).
 * https://oeis.org/A060681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - n / spf(n) (spf = smallest prime factor of n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t
smallest_prime_factor(idx_t m)
{
    if (m <= 1)
        return 1;          /* not used for n = 1, but keeps the function pure */

    if ((m & 1) == 0)      /* even */
        return 2;

    for (idx_t i = 3; (i * i) <= m; i += 2)
        if ((m % i) == 0)
            return i;

    return m;              /* m is prime */
}

seq_t
a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0 by convention */

    idx_t p = smallest_prime_factor(n);
    seq_t N = (seq_t)n;
    seq_t P = (seq_t)p;
    seq_t Q = N / P;
    return N - Q;          /* largest difference between consecutive divisors */
}
