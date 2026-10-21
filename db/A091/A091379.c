/*
 * A091379  a(n) = Product_{ p | n } (1 + Legendre(-1,p) ).
 * https://oeis.org/A091379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p | n} (1 + Legendre(-1,p)) where Legendre(-1,2)=1,
 *            Legendre(-1,p)=1 if p≡1 (mod 4), Legendre(-1,p)=-1 if p≡3 (mod
 *            4)
 * Range:     u8 n <= 9995, u16 n <= 9995, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    if (n == 1) return 1;

    idx_t temp = n;
    seq_t result = 1;

    /* handle prime factor 2 separately */
    if (temp & 1ULL)
    {
        /* temp even */
    }
    if ((temp % 2) == 0)
    {
        result *= 2;          /* factor for p=2 */
        while ((temp % 2) == 0) temp /= 2;
    }

    /* trial division over odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            if ((p % 4) == 3) return 0;    /* zero factor */
            result *= 2;                   /* factor 2 for this distinct prime */
            while (temp % p == 0) temp /= p;
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1)
    {
        if ((temp % 4) == 3) return 0;
        result *= 2;
    }

    return result;
}
