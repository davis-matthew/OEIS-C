/*
 * A081325  a(n) = sigma(n^2) modulo 4.
 * https://oeis.org/A081325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n^2) mod 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t res = 1;                /* product of prime factors modulo 4 */

    /* trial division up to sqrt(m) */
    for (idx_t i = 2; i <= m / i; ++i)
    {
        if (m % i == 0)
        {
            int e = 0;
            while (m % i == 0)
            {
                m /= i;
                ++e;
            }

            seq_t factor;
            if (i == 2)               /* prime 2 */
            {
                /* e > 0 implies factor 3 mod 4 */
                factor = 3;
            }
            else
            {
                if (i % 4 == 1)      /* p ≡ 1 (mod 4) */
                    factor = (seq_t)((2 * e + 1) & 3);  /* (2e+1) mod 4 */
                else                 /* p ≡ 3 (mod 4) */
                    factor = 1;       /* contributes 1 mod 4 */
            }

            res = (res * factor) & 3;   /* keep only modulo 4 */
        }
    }

    /* remaining prime factor > sqrt(original n), exponent = 1 */
    if (m > 1)
    {
        if (m == 2)
            res = (res * 3) & 3;
        else if (m % 4 == 1)   /* odd prime ≡ 1 (mod 4) */
            res = (res * 3) & 3;   /* 2*1+1 = 3 */
        else                   /* odd prime ≡ 3 (mod 4) */
            /* factor 1, no change */
            ;
    }

    return res;
}
