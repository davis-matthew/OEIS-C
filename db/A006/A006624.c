/*
 * A006624  a(n) is the least k such that every n X (n+3) {0,1}-matrix with k
 *          ones contains an all-ones 2 X 4 submatrix.
 * https://oeis.org/A006624
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n+1)(n+4)/2 for n <= 5; a(n) = (n+1)(n+4)/2 - F(n-3) for
 *            n >= 6
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 9 (2026-11-01)
 * Helpers:   fibonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fibonacci.h"

seq_t a(idx_t n) {
    seq_t result = ((seq_t)(n+1) * (seq_t)(n+4)) / 2;
    
    if (n >= 6) {
        result -= fibonacci(n - 3);
    }
    
    return result;
}
