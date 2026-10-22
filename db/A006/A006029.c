/*
 * A006029  Numbers k such that the k-th tetrahedral number k*(k+1)*(k+2)/6 is
 *          palindromic.
 * https://oeis.org/A006029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = n-th k such that k*(k+1)*(k+2)/6 is palindromic
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_palindromic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindromic.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    
    while (count < n) {
        k++;
        seq_t tetrahedral = ((seq_t)k * ((seq_t)k + 1) * ((seq_t)k + 2)) / 6;
        if (is_palindromic(tetrahedral)) {
            count++;
        }
    }
    
    return (seq_t)k;
}
