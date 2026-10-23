/*
 * A016026  Smallest base relative to which n is palindromic.
 * https://oeis.org/A016026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest base b > 1 in which n is represented as a palindrome.
   This is OEIS sequence A016026.                                                          */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* Every integer n has a palindrome representation in base n+1
       (a single digit).  Therefore the loop below always terminates. */
    for (idx_t base = 2;; ++base)
    {
        if (is_palindrome((seq_t)n, base))
            return (seq_t)base;

        /* When base > n the representation of n is a single digit,
           which is a palindrome.  This also covers the special
           small values n = 1, 2. */
        if (base > n)
            return (seq_t)base;
    }
}
