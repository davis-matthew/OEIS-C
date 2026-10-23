/*
 * A036750  Number of strings of n distinct digits from 1-9 that are the last
 *          n digits of a square in base 7.
 * https://oeis.org/A036750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = 3 * 5! / (6 - n)! for 1 ≤ n ≤ 6, a(n) = 0 for n = 0 or n
 *            > 6
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n > 6)
        return (seq_t)0;

    static const seq_t fact5 = 120;          /* 5! = 120 */

    idx_t m = 6 - n;                         /* (6 - n)!.  m is 0..5 here. */
    seq_t denom = 1;
    for (idx_t i = 1; i <= m; ++i)
        denom *= (seq_t)i;                  /* compute (6 - n)!   */

    return (seq_t)3 * fact5 / denom;        /* integer division is exact */
}
