/*
 * A178226  Characteristic function of A154809 (numbers that are not binary
 *          palindromes).
 * https://oeis.org/A178226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1 if n’s binary representation is not a palindrome, 0
 *            otherwise
 * Range:     u8 n <= 9763, u16 n <= 9763, u32 n <= 9763, u64 n <= 9763,
 *            u128 n <= 9763, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t and test binary palindrome */
    seq_t x = (seq_t)n;
    /* is_palindrome returns bool: true if palindrome. */
    return (seq_t)(!is_palindrome(x, 2));
}
