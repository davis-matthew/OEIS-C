/*
 * A028244  a(n) = 4^(n-1) - 3*3^(n-1) + 3*2^(n-1) - 1 (essentially Stirling
 *          numbers of second kind).
 * https://oeis.org/A028244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n-1) - 3*3^(n-1) + 3*2^(n-1) - 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_custom(seq_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    /* n >= 1 according to the OEIS offset */
    idx_t e = n - 1;
    seq_t p4 = pow_seq_custom(4, e);
    seq_t p3 = pow_seq_custom(3, e);
    seq_t p2 = pow_seq_custom(2, e);
    seq_t ans = p4 - (seq_t)3 * p3 + (seq_t)3 * p2 - 1;
    return ans;
}
