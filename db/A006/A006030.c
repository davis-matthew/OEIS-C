/*
 * A006030  Palindromic tetrahedral numbers.
 * https://oeis.org/A006030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th palindromic tetrahedral number, where tetrahedral
 *            numbers are T(k) = k(k+1)(k+2)/6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    seq_t tet;
    
    while (count < n) {
        k++;
        seq_t k_seq = (seq_t)k;
        tet = (k_seq * (k_seq + 1) / 2) * (k_seq + 2) / 3;
        if (is_palindrome(tet)) {
            count++;
        }
    }
    
    return tet;
}
