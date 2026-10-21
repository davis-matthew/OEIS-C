/*
 * A061916  Obtain m by omitting trailing zeros from n; a(n) = smallest k such
 *          that k*m is a palindrome with even digits, or -1 if no such
 *          multiple exists.
 * https://oeis.org/A061916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = smallest k such that k*m is an even‑digit palindrome,
 *            where m is n with trailing zeros removed. a(n) = -1 if m ends
 *            with the digit 5
 * Range:     i8 n <= 6, i16 n <= 26, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   all_even_digits, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <limits.h>
#include "../utils/is_palindrome.h"
#include "../utils/all_even_digits.h"

/* Return the sequence value for index n. */
seq_t a(idx_t n)
{
    /* Remove trailing zeros from n to obtain m. */
    seq_t m = (seq_t)n;
    while (m != 0 && (m % 10) == 0)
        m /= 10;

    /* Handle the special cases. */
    if (m == 0)          /* n = 0  →  0 * 1 = 0, a palindrome of even digits. */
        return (seq_t)1;
    if ((m % 10) == 5)   /* the negative condition from the OEIS definition. */
        return (seq_t)-1;

    /* Search for the smallest integer k ≥ 1 such that k*m is a palindrome
       made only of even digits. */
    seq_t k = 1;
    for (;; ++k) {
        /* Compute k*m in a 128‑bit unsigned type to avoid overflow when
           k or m are large.  For types larger than 64 bits the multiplication
           remains precise.  For 64‑bit types the cast is safe because we
           break the loop long before the product exceeds the representable
           range (the terms that fit in 64 bits are found by then).  */
        unsigned __int128 prod = (unsigned __int128)k * (unsigned __int128)m;
        seq_t p = (seq_t)prod;

        /* Check both the palindrome property and that all digits are even. */
        if (is_palindrome(p, 10) && all_even_digits(p))
            return k;
    }
    /* Unreachable, but required for completeness. */
    return (seq_t)-1;
}
