/*
 * A065018  a(n) = Sum_{d|n} sigma(d)^2.
 * https://oeis.org/A065018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = Sum_{d|n} sigma(d)^2
 * Range:     u8 n <= 7, u16 n <= 89, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where sigma(m) = Sum_{e|m} e */

seq_t a(idx_t n) {
    seq_t result = 0;

    /* iterate over divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            seq_t s = 0;
            for (idx_t e = 1; e * e <= d; ++e) {
                if (d % e == 0) {
                    s += e;
                    if (e * e != d) s += d / e;
                }
            }
            result += s * s;

            /* paired divisor n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                s = 0;
                for (idx_t e = 1; e * e <= d2; ++e) {
                    if (d2 % e == 0) {
                        s += e;
                        if (e * e != d2) s += d2 / e;
                    }
                }
                result += s * s;
            }
        }
    }

    return result;
}
