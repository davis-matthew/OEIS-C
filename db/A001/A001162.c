/*
 * A001162  Numbers containing an odd number of letters.
 * https://oeis.org/A001162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = n-th positive integer whose English word representation
 *            contains an odd number of letters
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   letter_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/letter_count.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    
    while (count < (seq_t)n) {
        num++;
        if (letter_count(num) % 2 == 1) {
            count++;
        }
    }
    
    return num;
}
