/*
 * A006595  a(n) = (n+2)!/4 + n!/2.
 * https://oeis.org/A006595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)!/4 + n!/2 = (n!/2) * ((n+1)*(n+2)/2 + 1), a(0) = 1,
 *            a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    /* f = n!/2 = 3*4*...*n */
    seq_t f = (seq_t)1;
    for (idx_t k = 3; k <= n; k++) {
        f *= (seq_t)k;
    }
    /* h = (n+1)(n+2)/2, exact in 64 bits for practical n */
    uint64_t h64 = ((uint64_t)(n + 1) * (uint64_t)(n + 2)) / 2;
    seq_t h = (seq_t)h64;
    return f * h + f;
}
