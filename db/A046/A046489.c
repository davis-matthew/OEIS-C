/*
 * A046489  Sum of the first n palindromes (A002113).
 * https://oeis.org/A046489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the first (n+1) base‑10 palindromes
 * Range:     u8 n <= 13, u16 n <= 117, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 896 (2026-11-01)
 * Helpers:   is_palindrome_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome_base.h"

/* Compute the sum of the first (n+1) palindromes. */
seq_t a(idx_t n)
{
    seq_t sum   = 0;   /* running total */
    idx_t count = 0;   /* how many palindromes have been added */

    for (idx_t i = 1; ; ++i)
    {
        if (is_palindrome_base((seq_t)i, 10))
        {
            sum += (seq_t)i;
            ++count;
            if (count > n)  /* added (n+1) palindromes, done */
                break;
        }
    }
    return sum;
}
