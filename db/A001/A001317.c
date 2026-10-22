/*
 * A001317  Sierpiński's triangle (Pascal's triangle mod 2) converted to
 *          decimal.
 * https://oeis.org/A001317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice, hear
 * Formula:   a(n) = Product_{j where bit j of n is 1} (2^(2^j) + 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2pow2.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t bit_pos = 0;
    
    while (n > 0) {
        if (n & 1) {
            seq_t fermat = pow2pow2(bit_pos) + 1;
            result = result * fermat;
        }
        n >>= 1;
        bit_pos++;
    }
    
    return result;
}
