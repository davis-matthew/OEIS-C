/*
 * A016160  Duplicate of A005062.
 * https://oeis.org/A016160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 6^n - 5^n (offset 0)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 10, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* 6^0 - 5^0 = 0, 6^1 - 5^1 = 1, 6^2 - 5^2 = 11, ... */
    seq_t pow6 = pow_seq((seq_t)6, n);
    seq_t pow5 = pow_seq((seq_t)5, n);
    return pow6 - pow5;
}
