/*
 * A004654  Powers of 2 written in base 15. (Next term contains a non-decimal
 *          character.)
 * https://oeis.org/A004654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2^n written in base 15, digits concatenated as decimal
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"

seq_t a(idx_t n) {
    seq_t power = A001832_pow2(n);
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (power > 0) {
        seq_t digit = power % 15;
        result = result + digit * multiplier;
        multiplier *= 10;
        power /= 15;
    }
    
    return result;
}
