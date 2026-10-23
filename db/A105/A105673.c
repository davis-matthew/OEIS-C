/*
 * A105673  One-half of theta series of square lattice (or half the number of
 *          ways of writing n > 0 as a sum of 2 squares), without the constant
 *          term, which is 1/2.
 * https://oeis.org/A105673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * ∏_{p≡1 (mod 4)} (e_p + 1) if any prime ≡3 (mod 4) has
 *            odd exponent, then a(n)=0. a(n) is one‑half of the number of
 *            representations of n as a sum of two squares
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1, but harmless for n=0 */

    idx_t m = n;

    /* Remove all factors of 2 – they do not affect the count */
    while ((m & 1) == 0)
        m >>= 1;

    seq_t prod = 1;   /* product of (e_p + 1) for primes ≡1 (mod 4) */

    /* Factor odd primes */
    for (idx_t p = 3; p <= m / p; p += 2)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            if ((p & 3) == 1)        /* p ≡ 1 (mod 4) */
            {
                prod *= (seq_t)(e + 1);
            }
            else if ((p & 3) == 3)   /* p ≡ 3 (mod 4) */
            {
                if (e & 1)            /* odd exponent – no representations */
                    return 0;
                /* even exponent – contributes nothing to prod */
            }
            /* primes ≡ 2 (mod 4) cannot occur here because p is odd */
        }
    }

    /* Any remaining prime factor greater than sqrt(original n) */
    if (m > 1)
    {
        if ((m & 3) == 1)          /* m ≡ 1 (mod 4) */
        {
            prod *= 2;             /* exponent is 1 → (1+1) */
        }
        else if ((m & 3) == 3)     /* m ≡ 3 (mod 4) */
        {
            return 0;              /* odd exponent 1 */
        }
        /* primes ≡ 2 (mod 4) impossible for odd m */
    }

    /* Multiply by 2 to obtain r₂(n)/2 */
    return prod * 2;
}
