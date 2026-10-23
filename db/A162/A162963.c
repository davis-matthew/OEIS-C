/*
 * A162963  a(n) = 5*a(n-2) for n > 2; a(1) = 2, a(2) = 5.
 * https://oeis.org/A162963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) ; initial values: a(1)=2, a(2)=5
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_five.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)5;
    idx_t half = n / (idx_t)2;
    seq_t pow5 = pow_five(half);
    if ((n & (idx_t)1) == 0)          /* even n */
        return pow5;
    else                              /* odd n */
        return pow5 * (seq_t)2;
}
