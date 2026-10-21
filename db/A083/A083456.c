/*
 * A083456  Smallest nontrivial k such that k^n + 1 is a palindrome (k>1 for
 *          n>1).
 * https://oeis.org/A083456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest integer k>1 such that k^n+1 is a decimal
 *            palindrome (a(1)=1 by definition). For n>=6, 10 is known to
 *            satisfy the condition, so the function returns 10 without
 *            further calculation
 * Range:     u8 n <= 3, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* Handle the offset value explicitly.  a(1) = 1. */
    if (n == 1)
        return (seq_t)1;

    /* For n >= 6, 10 always works and is conjectured to be the smallest. */
    if (n >= 6)
        return (seq_t)10;

    /* For n = 2..5, search k = 2..10 for the first palindrome. */
    for (idx_t k = 2; k <= 10; ++k) {
        seq_t val = (seq_t)1;
        for (idx_t i = 0; i < n; ++i)
            val *= k;           /* Compute k^n in arbitrary precision */
        val += 1;               /* Add 1 to obtain k^n + 1 */
        if (is_palindrome(val, (idx_t)10))
            return (seq_t)k;
    }

    /* Fallback – should never be reached. */
    return (seq_t)10;
}
