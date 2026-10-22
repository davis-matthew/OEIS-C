/*
 * A007775  Numbers not divisible by 2, 3 or 5.
 * https://oeis.org/A007775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 3 - 2*floor((n-1)/8) + (1 +
 *            (-1)^floor((n-2)/2))*(-1)^floor((n-2)/4)
 * Range:     u8 n <= 68, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n) {
    int64_t n_s = (int64_t)n;
    
    // Compute floor divisions
    int64_t f1 = (n_s - 1) / 8;
    
    int64_t nm2 = n_s - 2;
    int64_t f2 = nm2 / 2;
    int64_t f3 = nm2 / 4;
    
    // Correct for floor division with negative numbers
    if (nm2 < 0 && nm2 % 2 != 0) f2--;
    if (nm2 < 0 && nm2 % 4 != 0) f3--;
    
    // Main formula: 4*n - 3 - 2*f1 + (1 + (-1)^f2) * (-1)^f3
    seq_t result = 4 * (seq_t)n - 3 - 2 * (seq_t)f1;
    
    // Add the correction term: (1 + (-1)^f2) * (-1)^f3
    int s1 = (f2 & 1) ? -1 : 1;
    int s2 = (f3 & 1) ? -1 : 1;
    result = result + (seq_t)((1 + s1) * s2);
    
    return result;
}
