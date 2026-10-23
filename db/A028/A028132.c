/*
 * A028132  Expansion of 1/((1-4x)(1-6x)(1-7x)(1-10x)).
 * https://oeis.org/A028132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^(n+3)-8*7^(n+3)+9*6^(n+3)-2*4^(n+3))/72
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* fast exponentiation for arbitrary seq_t types */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* compute 10^(n+3), 7^(n+3), 6^(n+3), 4^(n+3) */
    idx_t e = n + 3;
    seq_t p10 = pow_seq((seq_t)10, e);
    seq_t p7  = pow_seq((seq_t)7, e);
    seq_t p6  = pow_seq((seq_t)6, e);
    seq_t p4  = pow_seq((seq_t)4, e);

    /* combine according to the formula in a way that avoids unsigned underflow */
    seq_t num = p10 + (seq_t)9 * p6;   /* p10 + 9*6^(n+3) */
    num -= (seq_t)8 * p7;              /* - 8*7^(n+3) */
    num -= (seq_t)2 * p4;              /* - 2*4^(n+3) */

    return num / (seq_t)72;
}
