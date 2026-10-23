/*
 * A037715  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,3,2,0.
 * https://oeis.org/A037715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the decimal integer formed by the first n terms of the
 *            repeating pattern 1,3,2,0. That is a(n) = d1 d2 ... dn in base
 *            10, where d_k = {1,3,2,0}[ (k-1) mod 4 ]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t etc, if needed by build */

seq_t a(idx_t n) {
    seq_t res = 0;
    const unsigned char pat[4] = {1, 3, 2, 0};
    for (idx_t i = 0; i < n; ++i) {
        res = res * 10 + pat[i & 3];
    }
    return res;
}
