/*
 * A072695  a(n) = lcm(d(n^2),d(n)), where d(n) = A000005, the number of
 *          divisors of n.
 * https://oeis.org/A072695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(d(n^2), d(n))
 * Range:     u8 n <= 47, u16 n <= 9786, u32 n <= 9786, u64 n <= 9786,
 *            u128 n <= 9786, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t gcd_seq(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    /* n ≥ 1 */
    uint64_t t = (uint64_t)n;
    seq_t d  = 1;  /* divisor count of n */
    seq_t d2 = 1;  /* divisor count of n² */
    uint64_t temp = t;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            d  *= (seq_t)(exp + 1);
            d2 *= (seq_t)(2 * exp + 1);
        }
    }
    if (temp > 1) {          /* remaining prime factor with exponent 1 */
        d  *= 2;
        d2 *= 3;
    }
    seq_t g = gcd_seq(d, d2);
    return (d / g) * d2;
}
