/*
 * A052036  Smallest number that must be added to n to make or keep n
 *          palindromic.
 * https://oeis.org/A052036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   is_palindrome_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = smallest k ≥ 0 such that n + k is a decimal palindrome */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome_seq_t.h"

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    if (is_palindrome_seq_t(val))
        return 0;                       /* already a palindrome */

    seq_t m = val + 1;
    for (;; ++m)
    {
        if (is_palindrome_seq_t(m))
            return m - val;              /* smallest k to reach palindromic n */
    }
}
