/*
 * A098732  Consider the sequence {b(n), n >= 1} of digits of the integers: 0
 *          1 2 3 4 5 6 7 8 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9 2 0...
 *          (A033307); a(n) = n - b(n).
 * https://oeis.org/A098732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n - d(n) where d(n) is the nth digit in the infinite
 *            string that consists of the decimal representations of the
 *            non‑negative integers concatenated in order
 *            (0,1,2,3,4,5,6,7,8,9,10,11,12,…)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   nth_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>
#include "../utils/nth_digit.h"

seq_t a(idx_t n)
{
    seq_t digit = nth_digit(n);
    return (seq_t)n - digit;
}
