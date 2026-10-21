/*
 * A060871  Number of n X n matrices over GF(7) with rank 1.
 * https://oeis.org/A060871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^n - 1)^2 / 6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p = pow_seq_t(7, n);         /* compute 7^n */
    seq_t m = p - 1;                   /* 7^n - 1 */
    seq_t num = m * m;                 /* (7^n - 1)^2 */
    return num / 6;                    /* divide by 6 */
}
