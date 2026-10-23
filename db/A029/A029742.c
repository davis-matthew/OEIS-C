/*
 * A029742  Nonpalindromic numbers.
 * https://oeis.org/A029742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice, changed
 * Formula:   not stated
 * Range:     u8 n <= 221, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029742: nth non‑palindromic decimal integer (offset = 1).  */
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)10;   /* first candidate after 9 (all single‑digit numbers are palindromic) */
    idx_t cnt = 0;           /* count of non‑palindromic numbers seen */

    while (1) {
        if (!is_palindrome(val, (idx_t)10)) {   /* check decimal palindrome */
            ++cnt;
            if (cnt == n)
                return val;                      /* nth non‑palindromic number found */
        }
        ++val;                                   /* move to next integer */
    }
}
