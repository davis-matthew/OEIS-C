/*
 * A124404  Nonpalindromes in base 10 that are non-palindromes in base 2.
 * https://oeis.org/A124404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth non‑palindrome in base 10 that is also a non‑palindrome in base 2.
   The sequence is defined by searching over all integers until the nth number
   satisfying both conditions is found.
   Requires:  #include "../utils/is_palindrome.h"  */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* Offset 1: the first term is the first number that satisfies the
       two non‑palindrome conditions.  For n == 0 we return 0 as a
       harmless placeholder. */
    if (n == 0)
        return (seq_t)0;

    idx_t  count = 0;
    seq_t  num   = 0;

    while (count < n) {
        num++;  /* next candidate integer */

        /* Skip numbers that are palindromes in base 10. */
        if (is_palindrome(num, 10))
            continue;

        /* Skip numbers that are palindromes in base 2. */
        if (is_palindrome(num, 2))
            continue;

        /* num satisfies both conditions. */
        count++;
    }

    return num;  /* num is the nth desired number. */
}
