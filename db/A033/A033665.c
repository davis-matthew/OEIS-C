/*
 * A033665  Number of 'Reverse and Add' steps needed to reach a palindrome
 *          starting at n, or -1 if n never reaches a palindrome.
 * https://oeis.org/A033665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   number of reverse-and-add steps needed to reach a decimal
 *            palindrome, or -1 if a palindrome is not found within the
 *            maximum allowed iterations
 * Range:     u8 n <= 38, u16 n <= 88, u32 n <= 88, u64 n <= 195,
 *            u128 n <= 195, big any n
 * Validated: standard, pass, 120 terms through n = 193 (2026-11-01)
 * Helpers:   is_palindrome_base, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/reverse_digits.h"

#define MAX_STEPS 1000

seq_t a(idx_t n)
{
    /* If n is already a palindrome, no steps are needed. */
    if (is_palindrome_base((seq_t)n, (idx_t)10))
        return (seq_t)0;

    seq_t cur = (seq_t)n;

    for (idx_t step = 1; step <= MAX_STEPS; ++step)
    {
        /* Reverse the decimal digits of cur. */
        seq_t rev = reverse_digits(cur);
        cur = cur + rev;
        if (is_palindrome_base(cur, (idx_t)10))
            return (seq_t)step;
    }
    /* Palindrome not found within the allowed range. */
    return (seq_t)-1;
}
