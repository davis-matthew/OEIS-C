/*
 * A088964  Number of solutions to x^2 == 2y^2 (mod n).
 * https://oeis.org/A088964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = number of solutions to x^2 ≡ 2y^2 (mod n). It is a
 *            multiplicative function with a(2^e) = 2^e a(p^e) = p^(2·⌊e/2⌋)
 *            for p ≡ 3 or 5 (mod 8) a(p^e) = ((p−1)·e + p)·p^(e−1) for p ≡ 1
 *            or 7 (mod 8)
 * Range:     u8 n <= 97, u16 n <= 9400, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute prime power contribution to the sequence value. */
static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= base;
    return r;
}

static seq_t prime_power_contrib(idx_t prime, idx_t exp) {
    if (prime == 2) {
        return pow_seq_t((seq_t)2, exp);
    }
    if (prime % 8 == 1 || prime % 8 == 7) {
        /* a(p^e) = ((p-1)*e + p) * p^(e-1) */
        seq_t coeff = ((seq_t)(prime - 1)) * exp + (seq_t)prime;
        seq_t powp = pow_seq_t((seq_t)prime, exp - 1);
        return coeff * powp;
    }
    /* prime ≡ 3 or 5 (mod 8) */
    idx_t k = exp / 2;          /* floor(exp/2) */
    return pow_seq_t((seq_t)prime, 2 * k);
}

seq_t a(idx_t n) {
    if (n <= 1) return (n == 1) ? 1 : 0;  /* a(1)=1, a(0) unused */

    idx_t m = n;
    seq_t result = 1;

    /* factor out powers of 2 */
    if ((m & 1) == 0) {
        idx_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        result *= pow_seq_t((seq_t)2, e);
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= prime_power_contrib(p, e);
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        result *= prime_power_contrib(m, 1);
    }

    return result;
}
