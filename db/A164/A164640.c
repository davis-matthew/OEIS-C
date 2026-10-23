/*
 * A164640  a(n) = 8*a(n-2) for n > 2; a(1) = 1, a(2) = 6.
 * https://oeis.org/A164640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) for n>2; a(1)=1, a(2)=6
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow8(idx_t exp) {
    seq_t result = 1;
    seq_t base = 8;
    while (exp) {
        if (exp & 1u)
            result = result * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)6;
    if (n & 1) {          /* odd index */
        idx_t k = (n - 1) / 2;
        return pow8(k);
    } else {              /* even index */
        idx_t k = n / 2;
        seq_t eight_pow = pow8(k - 1);
        return (seq_t)6 * eight_pow;
    }
}
