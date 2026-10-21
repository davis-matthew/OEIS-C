/*
 * A090273  Numbers with a palindromic digital product.
 * https://oeis.org/A090273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_product_is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n‑th integer whose product of decimal digits is a palindrome. */
#include <stdbool.h>
#include "../utils/digit_product_is_palindrome.h"

seq_t a(idx_t n)
{
    /* According to the OEIS entry, a(1) = 0. */
    if (n == 0) {
        return (seq_t)0;   /* undefined, but return 0 for safety */
    }

    idx_t found = 0;
    seq_t candidate = 0;

    while (1) {
        if (digit_product_is_palindrome(candidate)) {
            ++found;
            if (found == n) {
                return candidate;
            }
        }
        ++candidate;
    }
}
