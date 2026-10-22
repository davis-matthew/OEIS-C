/*
 * A007409  Denominators of Sum_{k=1..n} 1/k^3.
 * https://oeis.org/A007409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of Sum_{k=1..n} 1/k^3, computed by exact
 *            reduced fraction addition
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

static void reduce_by(seq_t *num, seq_t *den, idx_t r) {
    seq_t rr = (seq_t)r;
    while (*num % rr == 0 && *den % rr == 0) {
        *num /= rr;
        *den /= rr;
    }
}

seq_t a(idx_t n) {
    seq_t p = 0;
    seq_t q = 1;
    for (idx_t k = 1; k <= n; k++) {
        seq_t k3 = (seq_t)k * (seq_t)k * (seq_t)k;
        seq_t g = gcd_seq(q % k3, k3);
        seq_t qg = q / g;
        seq_t num = p * (k3 / g) + qg;
        seq_t den = qg * k3;
        /* any common factor of num and den must be a prime dividing k */
        idx_t m = k;
        for (idx_t r = 2; r * r <= m; r++) {
            if (m % r == 0) {
                while (m % r == 0) m /= r;
                reduce_by(&num, &den, r);
            }
        }
        if (m > 1) reduce_by(&num, &den, m);
        p = num;
        q = den;
    }
    return q;
}
