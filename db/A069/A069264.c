/*
 * A069264  Inverse Moebius transform of bigomega(n).
 * https://oeis.org/A069264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tau(n) * bigomega(n) / 2
 * Range:     u8 n <= 7559, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of prime factors (with multiplicity)
   summed over all divisors of n, for n>=1.
   Uses simple trial division – fast enough for n up to ~10000. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   // undefined for n=0, return 0 for safety

    seq_t tau = 1;                /* divisor count */
    seq_t bigomega = 0;           /* total number of prime factors */
    idx_t x = n;                  /* remaining part to factor */

    /* Factor by trial division */
    for (idx_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            int exp = 0;
            while (x % p == 0)
            {
                x /= p;
                ++exp;
                ++bigomega;
            }
            tau *= (seq_t)(exp + 1);
        }
    }

    /* If a prime factor larger than sqrt remains */
    if (x > 1)
    {
        ++bigomega;   /* one more prime factor */
        tau *= (seq_t)2;
    }

    /* a(n) = tau(n) * bigomega(n) / 2 */
    seq_t prod = tau * bigomega;
    return prod / 2;
}
