/*
 * A096962  a(n) = Sum_{0<d|n, n/d odd} d^9.
 * https://oeis.org/A096962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Σ_{0<d|n, n/d odd} d^9 For n = 2^k * m (m odd) → a(n) =
 *            2^(9k) * σ_9(m), where σ_9(m) = Σ_{u|m} u^9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary multiplication‑by‑power routine */
static inline seq_t pow_seq_local(seq_t base, int exp) {
    seq_t r = 1;
    while (exp) {
        if (exp & 1) r *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return r;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* out of offset */

    /* Separate powers of two */
    idx_t tmp = n;
    idx_t k = 0;                          /* exponent of 2 in n */
    while ((tmp & 1) == 0) {              /* n even → count factors of 2 */
        tmp >>= 1;                         /* divide by 2 */
        ++k;
    }

    /* Compute the 2^(9k) factor */
    seq_t res = pow_seq_local(2, 9 * (int)k);

    /* Now tmp is the odd part m */
    seq_t sigma = 1;                      /* σ_9(m) starts as 1 (divisor 1) */

    /* Factor tmp by trial division up to sqrt(tmp) */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            /* count exponent e of prime p */
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            /* compute Σ_{i=0}^{e} p^{9i} without huge intermediates */
            seq_t sum = 1;          /* i = 0 term */
            seq_t term = 1;          /* p^{9*i} starting at i = 0 */
            seq_t p9 = pow_seq_local(p, 9);
            for (idx_t i = 1; i <= e; ++i) {
                term *= p9;          /* term = p^{9i} */
                sum += term;         /* add to sum */
            }
            sigma *= sum;            /* multiply into σ_9(m) */
        }
    }

    /* If a prime factor > sqrt(tmp) remains */
    if (tmp > 1) {
        /* tmp is prime with exponent e = 1 */
        seq_t sum = 1 + pow_seq_local(tmp, 9);
        sigma *= sum;
    }

    res *= sigma;                         /* a(n) = 2^(9k) * σ_9(m) */
    return res;
}
