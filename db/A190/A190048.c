/*
 * A190048  Expansion of g.f. 2*(4+3*x)/(1-x)^5.
 * https://oeis.org/A190048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 264, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom_k4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_k4.h"
/*
 * Formula: a(n) = 8*binomial(n+4,4) + 6*binomial(n+3,4)
 * where binomial(k,4) = k*(k-1)*(k-2)*(k-3)/24 for k >= 4,
 *   and binomial(k,4) = 0 for k < 4.
 *
 * The implementation uses only integer arithmetic compatible with
 * C11 and C++17, and works for any seq_t that supports the usual
 * operators.  It runs in O(1) time per call and uses no dynamic
 * memory.
 */

static inline seq_t binom_k3(idx_t n) {
    /* Binomial coefficient C(n,4) with n replaced by (n+3) */
    seq_t a = (seq_t)(n + 3);
    seq_t b = (seq_t)(n + 2);
    seq_t c = (seq_t)(n + 1);
    seq_t d = (seq_t)(n);          /* n can be zero */
    return a * b * c * d / (seq_t)24;
}

seq_t a(idx_t n) {
    seq_t t1 = binom_k4(n);
    seq_t t2 = binom_k3(n);
    return (seq_t)8 * t1 + (seq_t)6 * t2;
}
