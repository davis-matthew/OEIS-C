/*
 * A006921  Diagonals of Pascal's triangle mod 2 interpreted as binary
 *          numbers.
 * https://oeis.org/A006921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{r=0..n/2} binomial(n-r,r)_{mod 2} * 2^(floor(n/2)-r)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t half_n = n / 2;
    
    for (idx_t r = 0; r <= half_n; r++) {
        idx_t n_minus_r = n - r;
        if ((n_minus_r & r) == r) {
            result = result + pw2(half_n - r);
        }
    }
    
    return result;
}
