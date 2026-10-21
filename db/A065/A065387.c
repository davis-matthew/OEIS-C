/*
 * A065387  a(n) = sigma(n) + phi(n).
 * https://oeis.org/A065387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) + phi(n)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute using trial division, suitable for n up to about 1e9 in <2 s. */

seq_t a(idx_t n)
{
    /* φ(n) = n ∏(1‑1/p) for all distinct prime factors p of n */
    /* σ(n) = ∏ (p^(e+1)‑1) / (p‑1)      for each p^e | n            */
    seq_t phi   = (seq_t)n;   /* start with n */
    seq_t sigma = (seq_t)1;   /* start with 1 */

    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* p is a prime factor */
            idx_t e = 0;
            seq_t p_pow = (seq_t)1;      /* p^e  */
            seq_t sum_p = (seq_t)1;      /* 1 + p + … + p^e */

            /* factor out p^e */
            while (m % p == 0) {
                m     /= p;
                ++e;
                p_pow  *= (seq_t)p;
                sum_p  += p_pow;
            }

            /* update φ and σ */
            phi   = phi / (seq_t)p * (seq_t)(p - 1);   /* φ *= (1‑1/p) */
            sigma *= sum_p;                            /* σ *= (1 + p + … + p^e) */
        }
    }

    /* if m > 1, m is a remaining prime factor */
    if (m > 1) {
        seq_t p = (seq_t)m;
        phi = phi / p * (p - 1);   /* φ *= (1‑1/p) */
        sigma *= (p + 1);           /* σ *= (1 + p) */
    }

    return phi + sigma;
}
