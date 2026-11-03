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
 * Validated: partial, pass, 75 terms through n = 74 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/num_digits_idx.h"

static seq_t A004180(idx_t n) {
    if (n == 0) return 0;
    
    idx_t digits[20];
    int num_digits = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[num_digits++] = temp % 10;
        temp /= 10;
    }
    
    idx_t result = 0;
    for (int i = num_digits - 1; i >= 0; i--) {
        if (digits[i] != 5) {
            result = result * 10 + digits[i];
        }
    }
    
    return (seq_t)result;
}

seq_t a(idx_t n) {
    int L = num_digits_idx(n);
    
    idx_t rep5 = 0;
    for (int i = 0; i < L; i++) {
        rep5 = rep5 * 10 + 5;
    }
    
    int bracket = (rep5 >= n + L) ? 1 : 0;
    int m = L - bracket;
    
    return A004180(n + (idx_t)m);
}
