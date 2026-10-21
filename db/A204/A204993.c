/*
 * A204993  Negative of the discriminant of quadratic field Q(sqrt(-n)).
 * https://oeis.org/A204993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (m % 4 == 3 ? m : 4 * m), where m is the product of
 *            primes dividing n with odd exponent (i.e., n divided by its
 *            largest square divisor)
 * Range:     u8 n <= 64, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t tmp = n;
    seq_t m = (seq_t)1;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            if (cnt & 1) {
                m *= (seq_t)p;
            }
        }
    }
    if (tmp > 1) {                 /* remaining prime factor */
        m *= (seq_t)tmp;
    }
    if ((m % (seq_t)4) == (seq_t)3) {
        return m;
    } else {
        return (seq_t)4 * m;
    }
}
