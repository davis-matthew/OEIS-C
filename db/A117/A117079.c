/*
 * A117079  a(1) = 1; a(n) = 2*a(n-1) + (number of digits in a(n-1)).
 * https://oeis.org/A117079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1) = 1; a(n) = 2*a(n-1) + digit_len(a(n-1)) for n >= 2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"   /* provides int digit_len(seq_t) */

seq_t a(idx_t n)
{
    /* The recurrence depends only on the previous term, so we can
       compute iteratively from 1 up to n. This is O(n) time and
       works for any seq_t that supports +, *, and int conversion. */
    seq_t prev = 1;            /* a(1) */
    if (n == 1) return prev;

    for (idx_t i = 2; i <= n; ++i)
    {
        int d = digit_len(prev);          /* number of decimal digits of a(i-1) */
        prev = prev * 2 + (seq_t)d;       /* a(i) = 2*a(i-1) + d */
    }
    return prev;
}
