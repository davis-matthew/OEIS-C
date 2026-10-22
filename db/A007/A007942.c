/*
 * A007942  Decimal concatenation of sequence (n, n-1, ..., 2, 1, 2, ..., n-1,
 *          n).
 * https://oeis.org/A007942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Decimal concatenation of sequence (n, n-1, ..., 1, 2, ..., n-1,
 *            n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   concat_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/concat_number.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t i = n; i > 0; i--) {
        result = concat_number(result, i);
    }
    
    for (idx_t i = 2; i <= n; i++) {
        result = concat_number(result, i);
    }
    
    return result;
}
