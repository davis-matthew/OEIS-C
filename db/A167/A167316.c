/*
 * A167316  Totally multiplicative sequence with a(p) = 6*(p-3) for prime p.
 * https://oeis.org/A167316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (6·(p-3))^e (a(3k)=0)
 * Range:     i8 n <= 7, i16 n <= 63, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, uint64_t exp) {
    seq_t result = 1;
    for (uint64_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    /* a(1) = 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* if n divisible by 3 the term is 0 */
    if ((n % 3) == 0) {
        return (seq_t)0;
    }

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* factor 2 separately */
    if ((m & 1ULL) == 0) {
        uint64_t exp = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++exp;
        }
        seq_t base = (seq_t)6 * ((seq_t)2 - 3); /* -6 */
        result *= pow_seq(base, exp);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {   /* p*p <= m */
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t base = (seq_t)6 * ((seq_t)p - 3);
            result *= pow_seq(base, exp);
        }
    }

    /* if a prime factor remains */
    if (m > 1) {
        seq_t base = (seq_t)6 * ((seq_t)m - 3);
        result *= base;
    }

    return result;
}
