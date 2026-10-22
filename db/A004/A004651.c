/*
 * A004651  Powers of 2 written in base 12. (Next term contains a non-decimal
 *          character.)
 * https://oeis.org/A004651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Base-12 digits of 2^n, each written in decimal and concatenated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"

seq_t a(idx_t n) {
    seq_t pow2 = A001832_pow2(n);
    
    seq_t digits[4096];
    idx_t len = 0;
    seq_t temp = pow2;
    
    while (temp > 0 && len < 4096) {
        digits[len++] = temp % 12;
        temp /= 12;
    }
    
    seq_t result = 0;
    for (idx_t i = len; i > 0; i--) {
        seq_t digit = digits[i - 1];
        if (digit <= 9) {
            result = result * 10 + digit;
        } else {
            result = result * 10 + 1;
            result = result * 10 + (digit - 10);
        }
    }
    
    return result;
}
