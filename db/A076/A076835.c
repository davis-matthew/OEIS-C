/*
 * A076835  Coefficients in expansion of Eisenstein series -q*E'_2.
 * https://oeis.org/A076835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24 * n * σ(n) (σ(n) = sum of divisors of n)
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sum of divisors σ(n) for n >= 1 */
static inline seq_t sigma_of(idx_t n)
{
    seq_t result = 1;          /* multiplicative identity */
    idx_t t = n;               /* working copy of n */

    /* trial division up to sqrt(t) */
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            /* compute 1 + p + p^2 + ... + p^e for this prime power */
            seq_t subtotal = 1;
            seq_t p_pow   = 1;
            while (t % p == 0) {
                t /= p;
                p_pow *= (seq_t)p;
                subtotal += p_pow;
            }
            result *= subtotal;
        }
    }

    /* if a prime factor larger than sqrt(n) remains */
    if (t > 1)
        result *= (seq_t)(1 + t);

    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; return 0 for safety */
        return 0;

    seq_t sigma = sigma_of(n);        /* σ(n) = sum of divisors */
    /* a(n) = 24 * n * σ(n) */
    return (seq_t)24 * (seq_t)n * sigma;
}
