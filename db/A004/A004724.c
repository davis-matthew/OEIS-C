/*
 * A004724  Delete all 5's from the sequence of nonnegative integers.
 * https://oeis.org/A004724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A004180(n + m) where m = L(n) - [555...5(L) >= n + L],
 *            L(n) is the number of decimal digits in n
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   A004180, count_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A004180.h"
#include "../utils/count_digits_idx.h"

seq_t a(idx_t n) {
    int L = count_digits_idx(n);
    
    idx_t rep5 = 0;
    for (int i = 0; i < L; i++) {
        rep5 = rep5 * 10 + 5;
    }
    
    int bracket = (rep5 >= n + L) ? 1 : 0;
    int m = L - bracket;
    
    return A004180(n + (idx_t)m);
}
