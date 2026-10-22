/*
 * A004642  Powers of 2 written in base 3.
 * https://oeis.org/A004642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = 2^n written in base 3, interpreted as a decimal number
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"

seq_t a(idx_t n) {
    seq_t power_of_2 = A001832_pow2(n);
    seq_t result = 0;
    seq_t power_of_10 = 1;
    
    while (power_of_2 > 0) {
        seq_t digit = power_of_2 % 3;
        result += digit * power_of_10;
        power_of_10 *= 10;
        power_of_2 /= 3;
    }
    
    return result;
}
