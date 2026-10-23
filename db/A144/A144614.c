/*
 * A144614  Sum of divisors of 3*n + 1.
 * https://oeis.org/A144614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(3*n + 1) (sum of divisors of 3*n + 1)
 * Range:     u8 n <= 44, u16 n <= 7932, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = 3*n + 1;          /* the argument whose divisors we sum */
    seq_t sum = 0;
    for (idx_t d = 1; d*d <= m; ++d) {
        if (m % d == 0) {
            sum += (seq_t)d;    /* add the divisor d */
            idx_t e = m / d;    /* the paired divisor */
            if (e != d)
                sum += (seq_t)e;/* add the paired divisor if distinct */
        }
    }
    return sum;
}
