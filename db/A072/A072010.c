/*
 * A072010  In prime factorization of n replace all primes of form k*4+1 with
 *          k*4+3 and primes of form k*4+3 with k*4+1.
 * https://oeis.org/A072010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, nice
 * Formula:   a(n) = Product_{p^e||n} (p + 2*(2 - p mod 4))^e, where a(1) = 1.
 *            For the prime 3 this gives 1 (since 3 + 2*(2-3) = 1) and for the
 *            prime 2 it gives 2. The function is completely multiplicative
 * Range:     u8 n <= 124, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* factor 2 */
    if (m % 2 == 0) {
        idx_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        seq_t t = (seq_t)2;          /* a(2) = 2 */
        for (idx_t i = 0; i < e; ++i)
            result *= t;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t t;
            if (p % 4 == 1)
                t = (seq_t)p + (seq_t)2;
            else            /* p % 4 == 3 (including 3 itself) */
                t = (seq_t)p - (seq_t)2;
            for (idx_t i = 0; i < e; ++i)
                result *= t;
        }
    }

    /* remaining prime factor larger than sqrt(n) */
    if (m > 1) {
        seq_t t;
        if (m % 4 == 1)
            t = (seq_t)m + (seq_t)2;
        else
            t = (seq_t)m - (seq_t)2;
        result *= t;
    }

    return result;
}
