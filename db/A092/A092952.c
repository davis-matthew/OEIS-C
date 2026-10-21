/*
 * A092952  Smallest unhappy number that takes n iterations of the sum of the
 *          squares of digits to reach 4, which is the smallest number of the
 *          unhappy numbers cycle.
 * https://oeis.org/A092952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092952:
   a(n) = a(n-8) for n > 8, with
   a(1)=4, a(2)=2, a(3)=11, a(4)=113, a(5)=78, a(6)=58,
   a(7)=29, a(8)=16.
   Thus for n ≥ 9, the values repeat with period 8:
   4, 2, 9, 3, 36, 6, 29, 16. */

#include <stddef.h>  /* for size_t, if desired */

static const seq_t _first[8] =
{ 4, 2, 11, 113, 78, 58, 29, 16 };

static const seq_t _cycle[8] =
{ 4, 2, 9, 3, 36, 6, 29, 16 };

seq_t a(idx_t n)
{
    if (n <= 8)
        return _first[n - 1];

    /* n >= 9: periodic part of length 8 */
    idx_t idx = (n - 9) % 8;
    return _cycle[idx];
}
