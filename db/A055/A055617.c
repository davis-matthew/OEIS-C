/*
 * A055617  a(n)= A*10^(3*n)+B with A=(10^(3*n)-10^n) and B=10^(2*n).
 * https://oeis.org/A055617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(6n) - 10^(4n) + 10^(2n)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* The OEIS offset is 1, so n>=1. */
    /* Compute the three powers of ten needed. */
    seq_t p2 = pow10(2 * n);   /* 10^(2n)  */
    seq_t p4 = pow10(4 * n);   /* 10^(4n)  */
    seq_t p6 = pow10(6 * n);   /* 10^(6n)  */

    /* a(n) = 10^(6n) - 10^(4n) + 10^(2n)  */
    seq_t result = p6 - p4 + p2;

    return result;
}
