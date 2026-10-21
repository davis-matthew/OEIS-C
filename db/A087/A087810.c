/*
 * A087810  First differences of A029931.
 * https://oeis.org/A087810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 1 if n mod 4 ≠ 0 * a(n) = 1 – T(v₂(n) – 1) if n mod 4 =
 *            0, * where T(k) = k*(k+1)/2 and v₂(n) is the exponent of 2 in n
 * Range:     i8 n <= 8985, i16 n <= 8985, i32 n <= 8985, i64 n <= 8985,
 *            i128 n <= 8985, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* If n is not a multiple of 4, the value is 1. */
    if ((n & 3) != 0) {
        return (seq_t)1;
    }

    /* Compute v₂(n) – the exponent of 2 in n. */
    unsigned int v2 = 0;
    idx_t temp = n;
    while ((temp & 1ULL) == 0) {
        temp >>= 1U;
        ++v2;
    }
    /* Since n is a multiple of 4, v2 >= 2. */

    /* Compute T(v₂(n) – 1) = (v2-1)*v2/2 as seq_t. */
    seq_t t = (seq_t)(v2 - 1) * (seq_t)v2 / (seq_t)2;

    /* Return 1 – T(v₂(n) – 1). */
    return (seq_t)1 - t;
}
