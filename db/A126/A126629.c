/*
 * A126629  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks digits 1 and 2, at
 *          least one of digits 3,4,5 and at least one of digits 6,7,8,9.
 * https://oeis.org/A126629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 12*6^n-30*5^n+34*4^n-21*3^n+7*2^n-1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_int(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* n is guaranteed >= 1 (offset 1) */
    seq_t p2 = pow_int((seq_t)2, n);
    seq_t p3 = pow_int((seq_t)3, n);
    seq_t p4 = pow_int((seq_t)4, n);
    seq_t p5 = pow_int((seq_t)5, n);
    seq_t p6 = pow_int((seq_t)6, n);

    seq_t val = (seq_t)12 * p6
              - (seq_t)30 * p5
              + (seq_t)34 * p4
              - (seq_t)21 * p3
              + (seq_t)7  * p2
              - (seq_t)1;

    return val;
}
