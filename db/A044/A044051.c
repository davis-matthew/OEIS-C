/*
 * A044051  a(n) = (s(n+1) + 1)/2, where s = A006995 (base-2 palindromes).
 * https://oeis.org/A044051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+1)/2 where s(n) is the nth base‑2 palindrome
 *            (A006995)
 * Range:     u8 n <= 30, u16 n <= 764, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   nth_binary_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nth_binary_palindrome.h"

seq_t a(idx_t n)
{
    seq_t pal = nth_binary_palindrome(n);
    return (pal + 1) / 2;
}
