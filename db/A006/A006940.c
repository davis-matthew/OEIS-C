/*
 * A006940  Rows of Pascal's triangle mod 3.
 * https://oeis.org/A006940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   rows of Pascal's triangle mod 3, interpreted as decimal numbers
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binom_mod_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_mod_3.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t k = 0; k <= n; k++) {
        int digit = binom_mod_3(n, k);
        result = result * (seq_t)10 + (seq_t)digit;
    }
    
    return result;
}
