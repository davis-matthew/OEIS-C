/*
 * A164861  Odd positive integers that are not palindromes when written in
 *          binary.
 * https://oeis.org/A164861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates the odd positive integers that are not binary
   palindromes.  It simply iterates through odd numbers, skips those whose
   binary representation is a palindrome (using the supplied helper
   `is_palindrome`), and returns the n‑th remaining odd integer. */
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* For offset 1; a(1) corresponds to the first term in the OEIS list. */
    if (n == 0) return 0;                /* undefined index, safe default */

    seq_t candidate = 1;                 /* start just before the first odd > 0 */
    idx_t count = 0;                     /* how many qualifying odds found */

    while (count < n)
    {
        candidate += (seq_t)2;           /* next odd number */
        if (!is_palindrome(candidate, 2))
            ++count;                    /* candidate is a non‑palindromic odd */
    }

    return candidate;
}
