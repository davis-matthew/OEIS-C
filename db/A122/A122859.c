/*
 * A122859  Expansion of phi(-q)^3 / phi(-q^3) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A122859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = 6 * Σ_{k|n} (-1)^k * b(n/k), where b(t) = 1 if t
 *            mod 3 = 1 -1 if t mod 3 = 2 0 if t mod 3 = 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t divisor_term(idx_t k, idx_t t) {
    /* Compute (-1)^k * b(t) where b(t) depends only on t % 3 */
    seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;
    idx_t r = t % 3;
    seq_t val;
    if (r == 1)          val = (seq_t)1;
    else if (r == 2)     val = (seq_t)-1;
    else                 val = (seq_t)0;
    return sign * val;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    /* Enumerate all divisors d of n. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor k = d, t = n/d */
            sum += divisor_term(d, n / d);
            if (d != n / d) {
                /* divisor k = n/d, t = d */
                sum += divisor_term(n / d, d);
            }
        }
    }
    return sum * (seq_t)6;
}
