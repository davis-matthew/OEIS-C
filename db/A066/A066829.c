/*
 * A066829  Parity of Omega(n): a(n) = 1 if n is the product of an odd number
 *          of primes; 0 if product of even number of primes.
 * https://oeis.org/A066829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if Ω(n) is odd, 0 if Ω(n) is even. Ω(n) = total number
 *            of prime factors of n counted with multiplicity. The
 *            implementation factors n by trial division, toggling a parity
 *            bit for every prime factor. This works for all n ≥ 1 and is fast
 *            enough for n up to about 10 000, with O(√n) time per...
 * Range:     u8 n <= 9799, u16 n <= 9799, u32 n <= 9799, u64 n <= 9799,
 *            u128 n <= 9799, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0 (empty factorisation has even parity) */
    if (n <= 1) return 0;

    seq_t parity = 0;      /* 0 for even, 1 for odd */
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int bit = 0;                  /* parity of this prime's exponent */
            while (m % p == 0)
            {
                m /= p;
                bit ^= 1;                  /* toggle for each occurrence */
            }
            parity ^= bit;                 /* XOR with accumulated parity */
        }
    }

    /* If anything remains, it's a prime factor > sqrt(original n). */
    if (m > 1)
    {
        parity ^= 1;                       /* single prime contributes odd exponent */
    }

    return parity;
}
