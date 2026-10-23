/*
 * A044922  Base 6 run lengths form a palindrome.
 * https://oeis.org/A044922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   base6_runlen_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer whose base-6 run-lengths form a palindrome */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/base6_runlen_palindrome.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num = 0;
    while (count < n) {
        ++num;
        if (base6_runlen_palindrome(num))
            ++count;
    }
    return (seq_t)num;
}
