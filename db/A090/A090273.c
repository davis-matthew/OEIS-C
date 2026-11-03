/*
 * A090273  Numbers with a palindromic digital product.
 * https://oeis.org/A090273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n‑th integer whose product of decimal digits is a palindrome. */
#include <stdbool.h>

static bool digit_product_is_palindrome(seq_t n)
{
    /* Compute the product of the decimal digits of n. */
    if (n == 0) {
        /* The digit product of 0 is 0, which is a palindrome. */
        return true;
    }

    seq_t prod = 1;
    seq_t temp = n;
    while (temp > 0) {
        int d = (int)(temp % 10);
        if (d == 0) {
            /* Any zero digit makes the product 0 (palindromic). */
            return true;
        }
        prod *= d;
        temp /= 10;
    }

    /* Check whether prod is a palindrome. */
    seq_t rev = 0;
    temp = prod;
    while (temp > 0) {
        rev = rev * 10 + (temp % 10);
        temp /= 10;
    }
    return rev == prod;
}

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
