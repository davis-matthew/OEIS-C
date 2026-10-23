/*
 * A167326  Totally multiplicative sequence with a(p) = 7*(p+3) for prime p.
 * https://oeis.org/A167326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Product over prime powers p^e dividing n of (7*(p+3))^e
 *            (Totally multiplicative with a(p) = 7*(p+3) for prime p)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */
    seq_t value = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* Factorise m by trial division */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* p is a prime factor, count its exponent */
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t factor = (seq_t)(7 * (p + 3));
            for (int i = 0; i < exp; ++i) value *= factor;
        }
    }
    /* If m > 1, it is a prime factor > sqrt(original n) */
    if (m > 1) {
        seq_t factor = (seq_t)(7 * (m + 3));
        value *= factor;                 /* exponent is 1 */
    }
    return value;
}
