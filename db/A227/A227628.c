/*
 * A227628  Number of Lipschitz quaternions X such that X^2 == X (mod n).
 * https://oeis.org/A227628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e of f(p,e) where * f(2,e) =
 *            2 (for any e ≥ 1); * f(p,e) = 2 + (p + 1) · p^(2e-1) for odd
 *            prime p. The sequence is * multiplicative with a(1) = 1
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t result = 1;
    idx_t m = n;

    /* Handle the factor 2 (any power of 2 contributes 2). */
    if ((m & 1) == 0) {
        while ((m & 1) == 0)
            m >>= 1;   /* divide out all factors of 2 */
        result *= (seq_t)2;
    }

    /* Factor the remaining odd part. */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* Compute p^(2e-1). */
            seq_t power = 1;
            int exp = 2 * e - 1;
            for (int i = 0; i < exp; ++i)
                power *= (seq_t)p;

            seq_t factor = (seq_t)2 + ((seq_t)(p + 1)) * power;
            result *= factor;
        }
    }

    /* If a prime > sqrt(n) remains, its exponent is 1. */
    if (m > 1) {
        seq_t power = (seq_t)m;              /* m^(2*1-1) = m */
        seq_t factor = (seq_t)2 + ((seq_t)(m + 1)) * power;
        result *= factor;
    }

    return result;
}
