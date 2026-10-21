/*
 * A203070  Smallest positive multiple of n that is palindromic in base 3,
 *          ignoring trailing zeros.
 * https://oeis.org/A203070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 196, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A203070: the smallest positive multiple of n that,
   after removing all trailing zeros in base 3, is a palindrome in base 3.
   The algorithm brute‑forces the multiplier (t = 1, 2, …) until the
   condition is met. */
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    idx_t t = 1;
    for (;; ++t) {
        /* compute multiple m = n * t */
        seq_t m = (seq_t)n * (seq_t)t;

        /* strip trailing zeros in base 3 */
        seq_t v = m;
        while (v % 3 == 0) {
            v /= 3;
        }

        /* check palindrome in base 3 */
        if (is_palindrome(v, 3)) {
            return m;
        }
    }
}
