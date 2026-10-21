/*
 * A065338  a(1) = 1, a(p) = p mod 4 for p prime and a(u * v) = a(u) * a(v)
 *          for u, v > 0.
 * https://oeis.org/A065338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nice, nonn
 * Formula:   a(n) = product over prime powers p^e||n of (p mod 4)^e (=
 *            ∏_{p|n} (p mod 4)^{v_p(n)}) , a(1)=1
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t res = (seq_t)1;
    idx_t m = n;

    /* handle factor 2 */
    if ((m & (idx_t)1) == 0) {
        unsigned int exp = 0;
        while ((m & (idx_t)1) == 0) {
            m >>= 1;
            ++exp;
        }
        seq_t base = (seq_t)2;          /* 2 mod 4 == 2 */
        while (exp--) res *= base;
    }

    /* trial division by odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            unsigned int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t base = (seq_t)(p % 4);
            while (exp--) res *= base;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        seq_t base = (seq_t)(m % 4);
        res *= base;
    }

    return res;
}
