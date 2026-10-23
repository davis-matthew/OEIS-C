/*
 * A183667  Number of (n+1) X 5 0..6 arrays with every 2 X 2 subblock summing
 *          to 12.
 * https://oeis.org/A183667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*7^n + 180*6^n + 900*5^n + 2280*4^n + 3960*3^n +
 *            5100*2^n + 4380
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    idx_t e = exp;           /* local copy to avoid modifying the argument */
    while (e > 0) {
        if (e & 1) result = result * base;
        e >>= 1;
        if (e) base = base * base;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t part1 = 7 * pow_seq(7, n);
    seq_t part2 = 180 * pow_seq(6, n);
    seq_t part3 = 900 * pow_seq(5, n);
    seq_t part4 = 2280 * pow_seq(4, n);
    seq_t part5 = 3960 * pow_seq(3, n);
    seq_t part6 = 5100 * pow_seq(2, n);
    seq_t sum = part1 + part2 + part3 + part4 + part5 + part6 + 4380;
    return sum;
}
