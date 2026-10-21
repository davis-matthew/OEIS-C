/*
 * A093733  Number of connected relations.
 * https://oeis.org/A093733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 63^n - 6*32^n - 15*18^n + 30*17^n - 10*14^n + 120*11^n -
 *            120*10^n + 30*9^n - 270*8^n + 360*7^n - 120*6^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1u) result *= base;
        base *= base;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Powers of each base */
    seq_t p63 = pow_seq_t((seq_t)63, n);
    seq_t p32 = pow_seq_t((seq_t)32, n);
    seq_t p18 = pow_seq_t((seq_t)18, n);
    seq_t p17 = pow_seq_t((seq_t)17, n);
    seq_t p14 = pow_seq_t((seq_t)14, n);
    seq_t p11 = pow_seq_t((seq_t)11, n);
    seq_t p10 = pow_seq_t((seq_t)10, n);
    seq_t p9  = pow_seq_t((seq_t)9,  n);
    seq_t p8  = pow_seq_t((seq_t)8,  n);
    seq_t p7  = pow_seq_t((seq_t)7,  n);
    seq_t p6  = pow_seq_t((seq_t)6,  n);

    /* Sum of positive terms */
    seq_t pos = 0;
    pos += p63;
    pos += 30 * p17;
    pos += 120 * p11;
    pos += 30 * p9;
    pos += 360 * p7;

    /* Sum of negative terms */
    seq_t neg = 0;
    neg += 6 * p32;
    neg += 15 * p18;
    neg += 10 * p14;
    neg += 120 * p10;
    neg += 270 * p8;
    neg += 120 * p6;

    /* Final result */
    return pos - neg;
}
