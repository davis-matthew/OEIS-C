/*
 * A076717  a(n) = -Sum_{d|n} (-n/d)^d.
 * https://oeis.org/A076717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{d|n} ( -n/d )^d
 * Range:     i8 n <= 14, i16 n <= 29, i32 n <= 59, i64 n <= 119,
 *            i128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A076717 (OEIS). */
#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1) result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t i;
    for (i = 1; (unsigned long long)i * (unsigned long long)i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d = i */
            seq_t base = -( (seq_t)n / (seq_t)i );
            seq_t term = pow_seq(base, i);
            sum += term;

            /* divisor d = n / i, if distinct */
            if (i != n / i) {
                idx_t d = n / i;
                base = -( (seq_t)n / (seq_t)d );
                term = pow_seq(base, d);
                sum += term;
            }
        }
    }
    return -sum;
}
