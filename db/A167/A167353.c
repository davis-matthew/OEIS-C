/*
 * A167353  Totally multiplicative sequence with a(p) = (p+1)*(p+3) = p^2+4p+3
 *          for prime p.
 * https://oeis.org/A167353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^e || n} ((p+1)*(p+3))^e, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t factor = (seq_t)(p + 1) * (seq_t)(p + 3);
            seq_t pow = (seq_t)1;
            for (uint64_t i = 0; i < exp; ++i)
                pow = pow * factor;
            result = result * pow;
        }
    }
    if (temp > 1) {
        seq_t factor = (seq_t)(temp + 1) * (seq_t)(temp + 3);
        result = result * factor;
    }
    return result;
}
