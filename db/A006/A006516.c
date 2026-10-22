/*
 * A006516  a(n) = 2^(n-1)*(2^n - 1), n >= 0.
 * https://oeis.org/A006516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = 2^(n-1)*(2^n - 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"


seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t pow2_2n_minus_1 = pw2(2 * n - 1);
    seq_t pow2_n_minus_1 = pw2(n - 1);
    
    return pow2_2n_minus_1 - pow2_n_minus_1;
}
