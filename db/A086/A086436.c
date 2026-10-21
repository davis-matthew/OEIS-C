/*
 * A086436  Maximum number of parts possible in a factorization of n; a(1) =
 *          1, and for n > 1, a(n) = A001222(n) = bigomega(n).
 * https://oeis.org/A086436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(n) – the total number of prime factors of n
 *            counting multiplicity. a(1) = 1, and for n ≥ 2 a(n) is the
 *            number of prime factors of n (with multiplicity)
 * Range:     u8 n <= 1024, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == (idx_t)1)
        return (seq_t)1;

    idx_t x = n;
    seq_t count = 0;

    /* Trial division up to sqrt(x).  The condition
       p <= x/p keeps the computation correct after x has been reduced. */
    for (idx_t p = 2; p <= x / p; ++p)
    {
        while (x % p == 0)
        {
            ++count;
            x /= p;
        }
    }

    /* If at the end x > 1, it is a prime factor with exponent 1. */
    if (x > 1)
        ++count;

    return count;
}
