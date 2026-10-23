/*
 * A015991  Numbers such that ten iterations of Reverse and Add are needed to
 *          reach a palindrome.
 * https://oeis.org/A015991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for numbers that require exactly ten reverse‑and‑add iterations to reach a palindrome. */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

static seq_t rev_decimal(seq_t v)
{
    seq_t rev = 0;
    while (v > 0)
    {
        rev = rev * 10 + (v % 10);
        v /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but return 0 to keep type */
        return 0;

    idx_t found = 0;
    idx_t cand_idx = 1; /* start testing from 1 */

    while (found < n)
    {
        seq_t current = (seq_t)cand_idx;
        seq_t x = current;
        idx_t steps = 0;
        bool palindrome_at_step10 = false;

        while (steps < 10)
        {
            seq_t rev = rev_decimal(x);
            x = x + rev;
            ++steps;
            if (is_palindrome(x, 10))
            {
                if (steps == 10)
                    palindrome_at_step10 = true;
                break;
            }
        }

        if (palindrome_at_step10)
        {
            ++found;
            if (found == n)
                return current;
        }

        ++cand_idx;
    }

    return 0; /* unreachable */
}
