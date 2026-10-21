/*
 * A206787  Sum of the odd squarefree divisors of n.
 * https://oeis.org/A206787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n, d odd, squarefree} d
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent multiplicative formula: a(n) = ∏_{p|odd_part(n)} (1 + p) */

seq_t a(idx_t n)
{
    /* Remove all factors of 2 – they do not contribute to odd divisors */
    while ((n & 1) == 0)
        n >>= 1;

    if (n == 1)
        return (seq_t)1;   /* n was a power of 2 */

    seq_t result = 1;
    idx_t x = n;   /* copy of the odd part to factor */

    /* Trial division by odd primes */
    for (idx_t p = 3; p <= x / p; p += 2)
    {
        if (x % p == 0)
        {
            result *= (seq_t)(p + 1);

            /* Remove all copies of this prime */
            while (x % p == 0)
                x /= p;
        }
    }

    /* If remainder > 1, it is a prime factor > sqrt(original n) */
    if (x > 1)
        result *= (seq_t)(x + 1);

    return result;
}
