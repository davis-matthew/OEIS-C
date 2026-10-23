/*
 * A189922  Jordan function J_{-4} multiplied by n^4.
 * https://oeis.org/A189922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Product_{p prime | n} (1 - p^4) (for n >= 2); a(1) = 1
 *            equivalent to a(n) = Sum_{d|n} µ(d) * d^4 Implementation uses
 *            trial division factorisation
 * Range:     i8 n <= 4, i16 n <= 13, i32 n <= 216, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            /* compute (1 - p^4) */
            seq_t p4 = (seq_t)p;
            p4 *= (seq_t)p;   /* p^2 */
            p4 *= (seq_t)p;   /* p^3 */
            p4 *= (seq_t)p;   /* p^4 */
            seq_t term = (seq_t)1 - p4;
            result *= term;
            /* remove all occurrences of p */
            while (m % p == 0) m /= p;
        }
    }
    /* if a prime factor > sqrt(n) remains */
    if (m > 1) {
        seq_t p4 = (seq_t)m;
        p4 *= (seq_t)m;   /* p^2 */
        p4 *= (seq_t)m;   /* p^3 */
        p4 *= (seq_t)m;   /* p^4 */
        seq_t term = (seq_t)1 - p4;
        result *= term;
    }
    return result;
}
