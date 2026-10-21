/*
 * A089810  Expansion of Jacobi theta function theta_3(Pi/6, q) in powers of
 *          q.
 * https://oeis.org/A089810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089810: Theta‑3(Pi/6, q) expansion
 *
 * a(0) = 1
 * For n > 0, write n = Π p_i ^ e_i.
 *   If e_i is odd for any p_i → a(n) = 0.
 *   Otherwise (all exponents even) multiply the factors:
 *      p = 2  →  factor = -1
 *      p = 3  →  factor = -2
 *      p > 3  →  factor = +1
 * So a(n) is simply -1^(v2) · (-2)^(v3) (all other prime powers contribute +1).
 * Only four values occur: 1, -1, -2, 2.  */ 

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* offset 0 */
    seq_t result = 1;

    /* factor 2 */
    if ((n & 1) == 0) {
        unsigned exp = 0;
        idx_t m = n;
        while ((m & 1) == 0) { m >>= 1; exp++; }
        if (exp & 1) return 0;     /* odd exponent → 0 */
        result *= (seq_t)-1;       /* even exponent → factor -1 */
    }

    /* factor 3 */
    if (n % 3 == 0) {
        unsigned exp = 0;
        idx_t m = n;
        while (m % 3 == 0) { m /= 3; exp++; }
        if (exp & 1) return 0;     /* odd exponent → 0 */
        result *= (seq_t)-2;       /* even exponent → factor -2 */
    }

    /* factor remaining primes > 3 */
    idx_t m = n;
    /* remove 2 and 3 again to get remaining factor */
    if ((m & 1) == 0) while (m % 2 == 0) m /= 2;
    if (m % 3 == 0) while (m % 3 == 0) m /= 3;

    for (idx_t p = 5; p * p <= m; p += 6) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) { m /= p; exp++; }
            if (exp & 1) return 0;   /* odd exponent */
            /* even exponent → factor +1 (no change to result) */
        }
        idx_t q = p + 2;
        if (q > m) break;
        if (m % q == 0) {
            unsigned exp = 0;
            while (m % q == 0) { m /= q; exp++; }
            if (exp & 1) return 0;   /* odd exponent */
            /* even exponent → factor +1 */
        }
    }

    if (m > 1) {
        /* Remaining factor is prime with exponent 1 (odd) */
        return 0;
    }

    return result;
}
