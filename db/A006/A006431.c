/*
 * A006431  Numbers that have a unique partition into a sum of four
 *          nonnegative squares.
 * https://oeis.org/A006431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4^((n-13)//3) * a(13 + (n-13)%3) for n >= 16, with base
 *            cases for n <= 15
 * Range:     u8 n <= 18, u16 n <= 30, u32 n <= 54, u64 n <= 102,
 *            u128 n <= 198, big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"


seq_t a(idx_t n) {
    static const seq_t base[] = {0, 1, 2, 3, 5, 6, 7, 8, 11, 14, 15, 23, 24, 32, 56};
    
    if (n <= 15) {
        return base[n - 1];
    }
    
    idx_t k = (n - 13) / 3;
    idx_t r = (n - 13) % 3;
    
    seq_t power_of_4 = A001832_pow2(2 * k);
    seq_t base_val = base[12 + r];
    
    return power_of_4 * base_val;
}
