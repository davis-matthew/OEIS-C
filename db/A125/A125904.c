/*
 * A125904  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks at least one of the
 *          digits 1,2,3,4 and at least one of the digits 5,6,7,8,9.
 * https://oeis.org/A125904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 20*8^n -70*7^n + 120*6^n -125*5^n + 84*4^n -36*3^n +
 *            9*2^n -1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_ui(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p2 = pow_ui(2, n);
    seq_t p3 = pow_ui(3, n);
    seq_t p4 = pow_ui(4, n);
    seq_t p5 = pow_ui(5, n);
    seq_t p6 = pow_ui(6, n);
    seq_t p7 = pow_ui(7, n);
    seq_t p8 = pow_ui(8, n);

    seq_t pos = (seq_t)20 * p8
              + (seq_t)120 * p6
              + (seq_t)84  * p4
              + (seq_t)9   * p2;

    seq_t neg = (seq_t)70 * p7
              + (seq_t)125 * p5
              + (seq_t)36  * p3
              + 1;

    return pos - neg;
}
