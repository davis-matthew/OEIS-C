/*
 * A133205  Fully multiplicative with a(p) = p*(p+1)/2 for prime p.
 * https://oeis.org/A133205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = ∏_{p^e‖n} ((p*(p+1))/2)^e (fully multiplicative,
 *            a(p)=p*(p+1)/2)
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t m = n;

    /* factor out all 2s */
    idx_t e = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        ++e;
    }
    if (e > 0) {
        seq_t t = ((seq_t)2) * ((seq_t)3) / 2;   /* T(2) = 3 */
        for (idx_t i = 0; i < e; ++i) result *= t;
    }

    /* trial division for odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t t = (seq_t)p * ((seq_t)p + 1) / 2;
            for (idx_t i = 0; i < e; ++i) result *= t;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t t = (seq_t)m * ((seq_t)m + 1) / 2;
        result *= t;
    }

    return result;
}
