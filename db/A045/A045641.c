/*
 * A045641  Palindromes that are divisible by 6.
 * https://oeis.org/A045641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 113, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   idx_pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate palindromes in increasing numeric order and
   return the n‑th palindrome that is divisible by 6.  This implements
   OEIS sequence A045641. */

#include <stdbool.h>
#include "../utils/idx_pow10.h"

/* Build a palindrome from its left half.
   left   – left half as an idx_t (most‑significant digit first)
   half   – number of digits in the left half
   odd    – true if the total length is odd (middle digit is the last
            digit of the left half) */
static seq_t make_pal(idx_t left, int half, bool odd)
{
    seq_t pal = (seq_t)left;          /* copy left half              */
    idx_t t   = left;                 /* digits to mirror             */
    if (odd) t /= 10;                 /* skip middle digit for odd len*/

    while (t > 0) {
        pal = pal * 10 + (seq_t)(t % 10);   /* append mirrored digit  */
        t  /= 10;
    }
    return pal;
}

seq_t a(idx_t n)
{
    idx_t count = 0;

    for (int len = 1; ; ++len) {
        int half = (len + 1) / 2;            /* digits in left half */

        idx_t start, end;
        if (len == 1) {
            start = 0;
        } else {
            start = idx_pow10(half - 1);    /* leading digit non‑zero */
        }
        end = idx_pow10(half) - 1;

        for (idx_t left = start; left <= end; ++left) {
            seq_t pal = make_pal(left, half, (len & 1) != 0);
            if (pal % 6 == 0) {
                ++count;
                if (count == n) return pal;
            }
        }
    }
}
