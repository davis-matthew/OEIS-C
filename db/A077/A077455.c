/*
 * A077455  a(n) = sigma_4(n^4)/sigma(n^4).
 * https://oeis.org/A077455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = σ₄(n⁴)/σ(n⁴) = product over prime powers pᵉ∥n of
 *            (p^(12e+3)+p^(8e+2)+p^(4e+1)+1) / (p³+p²+p+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 40, u128 n <= 1626,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute base^exp as a seq_t (big integer or fixed width) */
static inline seq_t pow_seq(seq_t base, int exp) {
    seq_t res = (seq_t)1;
    while (exp--) {
        res *= base;
    }
    return res;
}

/* Evaluate the multiplicative factor for a prime power p^e */
static inline seq_t prime_factor_term(idx_t p, int e) {
    seq_t p_seq = (seq_t)p;

    /* Pre‑compute needed powers of p */
    seq_t p_pow1  = p_seq;
    seq_t p_pow2  = pow_seq(p_seq, 2);
    seq_t p_pow3  = pow_seq(p_seq, 3);
    seq_t p_pow4  = pow_seq(p_seq, 4);
    seq_t p_pow8  = pow_seq(p_seq, 8);
    seq_t p_pow12 = pow_seq(p_seq, 12);

    /* Compute the terms:
       p^(12e+3) = p^3 * (p^12)^e
       p^(8e+2)  = p^2 * (p^8 )^e
       p^(4e+1)  = p^1 * (p^4 )^e
    */
    seq_t pow12e = pow_seq(p_pow12, e);
    seq_t pow8e  = pow_seq(p_pow8 , e);
    seq_t pow4e  = pow_seq(p_pow4 , e);

    seq_t term1 = pow12e * p_pow3;
    seq_t term2 = pow8e  * p_pow2;
    seq_t term3 = pow4e  * p_pow1;

    seq_t numerator = term1;
    numerator += term2;
    numerator += term3;
    numerator += (seq_t)1;

    seq_t denominator = p_pow3 + p_pow2 + p_pow1 + (seq_t)1;

    return numerator / denominator;   /* exact division */
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but safe */

    idx_t m  = n;
    idx_t p  = 2;
    seq_t result = (seq_t)1;

    while (p <= m / p) {               /* trial division */
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= prime_factor_term(p, e);
        }
        p = (p == 2) ? 3 : p + 2;      /* next odd candidate */
    }

    if (m > 1) {                         /* remaining prime factor */
        result *= prime_factor_term(m, 1);
    }

    return result;
}
