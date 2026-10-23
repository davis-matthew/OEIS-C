/*
 * A029828  Eisenstein series E_12(q) (alternate convention E_6(q)),
 *          multiplied by 691.
 * https://oeis.org/A029828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 691 * For n > 0, a(n) = 65520 * σ₁₁(n), * where σ₁₁(n) =
 *            ∑_{d|n} d¹¹, the divisor sum of exponent 11
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 20, u128 n <= 1161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)691;

    /* Compute σ₁₁(n) by prime factorisation. */
    idx_t temp = n;            /* temporary copy of n for factoring */
    seq_t sigma = (seq_t)1;    /* multiplicative identity */

    /* Trial division over all possible prime factors up to sqrt(temp). */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* Count exponent e of prime p in n. */
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* Compute the contribution of p^11 to σ₁₁(n). */
            /* First compute p¹¹. */
            seq_t p_pow = (seq_t)1;
            for (idx_t i = 0; i < 11; ++i)
                p_pow *= (seq_t)p;

            /* sum_{k=0..e} (p¹¹)^k = 1 + p¹¹ + p²² + … + p^(11e) */
            seq_t sum = (seq_t)0;
            seq_t term = (seq_t)1;
            for (idx_t k = 0; k <= e; ++k) {
                sum += term;
                term *= p_pow;
            }

            sigma *= sum;
        }
    }

    /* If a prime factor larger than sqrt(n) remains, it appears once. */
    if (temp > 1) {
        idx_t p   = temp;         /* remaining prime factor */
        seq_t p_pow = (seq_t)1;   /* p¹¹ */
        for (idx_t i = 0; i < 11; ++i)
            p_pow *= (seq_t)p;

        /* For exponent e = 1: sum = 1 + p¹¹. */
        sigma *= ((seq_t)1 + p_pow);
    }

    /* Final result: multiply by 65520. */
    return sigma * (seq_t)65520;
}
