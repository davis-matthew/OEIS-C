/*
 * A141708  Least positive multiple of 2n-1 which is palindromic in base 2.
 * https://oeis.org/A141708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(n) is the least positive multiple of (2n-1) that is a
 *            palindrome in base 2. This implements the definition by
 *            searching binary palindromes for the smallest one divisible by
 *            (2n-1). It works for any seq_t and idx_t provided by the build
 * Range:     u8 n <= 9, u16 n <= 98, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   smallest_palindrome_divisible
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/smallest_palindrome_divisible.h"

seq_t a(idx_t n)
{
    /* m = 2n-1 (odd).  Use seq_t for the multiplication
       but maintain m in the small integer form for mod. */
    seq_t m = (seq_t)(2 * n - 1u);
    return smallest_palindrome_divisible(m);
}
