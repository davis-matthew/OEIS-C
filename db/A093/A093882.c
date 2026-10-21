/*
 * A093882  Sum of all the numbers formed by deleting one digit from n.
 * https://oeis.org/A093882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = sum of all numbers obtained by deleting one digit from n
 * Range:     u8 n <= 237, u16 n <= 9726, u32 n <= 9726, u64 n <= 9726,
 *            u128 n <= 9726, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow10_seq(int exp) {
    seq_t r = 1;
    for (int i = 0; i < exp; ++i) r *= 10;
    return r;
}

seq_t a(idx_t n) {
    /* number of decimal digits in n */
    int d = 0;
    idx_t tmp = n;
    do { d++; tmp /= 10; } while (tmp);

    seq_t sum = 0;
    for (int i = 0; i < d; ++i) {
        seq_t pow_left  = pow10_seq(d - i);
        seq_t pow_right = pow10_seq(d - i - 1);
        seq_t left  = ((seq_t)n) / pow_left;
        seq_t right = ((seq_t)n) % pow_right;
        sum += left * pow_right + right;
    }
    return sum;
}
