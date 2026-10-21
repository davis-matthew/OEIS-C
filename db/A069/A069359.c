/*
 * A069359  a(n) = n * Sum_{p|n} 1/p where p are primes dividing n.
 * https://oeis.org/A069359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{p|n} 1/p (sum over distinct prime divisors of n)
 * Range:     u8 n <= 268, u16 n <= 9574, u32 n <= 9574, u64 n <= 9574,
 *            u128 n <= 9574, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                      /* Empty sum for n = 1 */

    seq_t result = 0;
    idx_t m = n;                        /* Remaining part to factor */

    /* Factor out 2 */
    if ((m & 1) == 0) {                 /* m % 2 == 0 */
        result += (seq_t)n / 2;
        while ((m & 1) == 0) m >>= 1;    /* m /= 2 */
    }

    /* Check odd candidates up to sqrt(m) */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            result += (seq_t)n / p;
            while (m % p == 0) m /= p;
        }
    }

    /* If any prime factor > sqrt(n) remains */
    if (m > 1)
        result += (seq_t)n / m;

    return result;
}
