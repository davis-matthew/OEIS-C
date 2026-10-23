/*
 * A180012  a(n) = smallest k >= 1 such that k and k*n have the same digit
 *          sum.
 * https://oeis.org/A180012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest k ≥ 1 such that digit_sum(k) == digit_sum(k*n)
 * Range:     u8 n <= 10, u16 n <= 1956, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* n = 0 is not in the sequence, but guard */
    seq_t nn = (seq_t)n;
    seq_t k = 1;
    while (1) {
        seq_t prod = k * nn;
        if (digit_sum(k) == digit_sum(prod))
            return k;
        ++k;
    }
}
