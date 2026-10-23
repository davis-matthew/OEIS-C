/*
 * A034681  Sum of seventh powers of unitary divisors.
 * https://oeis.org/A034681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^k∥n} (1 + p^(7*k)) (product over prime powers of n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* OEIS offset 1 */
    if (n == 1) return 1;          /* a(1) = 1 */

    seq_t result = 1;
    idx_t m = n;

    /* trial division up to sqrt(m) using overflow‑safe check */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* 1 + p^(7*exp) */
            seq_t term = big_pow((seq_t)p, (seq_t)(exp * 7));
            term = term + 1;
            result = result * term;
        }
    }

    /* remaining prime factor > sqrt(original n), if any */
    if (m > 1) {
        seq_t term = big_pow((seq_t)m, (seq_t)7);   /* exponent 7*1 */
        term = term + 1;
        result = result * term;
    }

    return result;
}
