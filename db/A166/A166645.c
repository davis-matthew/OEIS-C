/*
 * A166645  Totally multiplicative sequence with a(p) = 5*(p+1) for prime p.
 * https://oeis.org/A166645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e ∥ n} (5·(p+1))^e, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, int exp) {
    seq_t result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n) {
    if (n == 1)
        return 1;

    uint64_t m = (uint64_t)n;        /* work on a 64‑bit integer */
    seq_t result = 1;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t base = (seq_t)(5 * (p + 1));
            seq_t power = pow_seq(base, exp);
            result *= power;
        }
    }

    if (m > 1) {                     /* remaining prime factor */
        seq_t base = (seq_t)(5 * (m + 1));
        result *= base;
    }

    return result;
}
