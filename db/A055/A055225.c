/*
 * A055225  a(n) = Sum_{k divides n} (n/k)^k.
 * https://oeis.org/A055225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k divides n} (n/k)^k
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 59, u64 n <= 122,
 *            u128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A055225 by iterating over all divisors of n and
   evaluating the power using exponentiation by squaring. */

#include <stdint.h>

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t k1 = d;
            seq_t base1 = (seq_t)n / (seq_t)k1;
            sum += pow_seq_t(base1, k1);

            idx_t k2 = n / d;
            if (k2 != k1) {
                seq_t base2 = (seq_t)n / (seq_t)k2;
                sum += pow_seq_t(base2, k2);
            }
        }
    }
    return sum;
}
