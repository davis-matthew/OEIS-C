/*
 * A140807  a(n) is the largest integer such that n^k is palindromic in binary
 *          for all nonnegative integers k that are <= a(n).
 * https://oeis.org/A140807
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 115 terms through n = 9695 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A140807:
   a(n) = largest k such that n^m is a binary palindrome for all
   0 ≤ m ≤ k.
   For n ≤ 3 the values are a(2)=0, a(3)=3.
   For n > 3, a(n) is 1 iff n itself is a binary palindrome,
   otherwise a(n)=0. */

#include <stdbool.h>

static bool is_bin_palindrome(idx_t x)
{
    if (x <= 1)
        return true;                    /* 0 and 1 are palindromes */

    /* Find number of bits in x */
    int bitlen = 0;
    idx_t tmp = x;
    while (tmp > 0) {
        bitlen++;
        tmp >>= 1;
    }

    int left = 0;
    int right = bitlen - 1;
    while (left < right) {
        if ((((x >> left) & 1U) != ((x >> right) & 1U)))
            return false;
        left++;
        right--;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* by definition the sequence is defined for n ≥ 2 */
    if (n < 2)
        return (seq_t)0;

    if (n == 2)
        return (seq_t)0;          /* a(2) = 0 */

    if (n == 3)
        return (seq_t)3;          /* a(3) = 3 */

    /* For n > 3 a(n) is 1 iff n is a binary palindrome */
    return is_bin_palindrome(n) ? (seq_t)1 : (seq_t)0;
}
