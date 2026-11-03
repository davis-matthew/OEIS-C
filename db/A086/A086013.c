/*
 * A086013  Number of 5's in decimal expansion of n^2.
 * https://oeis.org/A086013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of digit '5' in the decimal representation of n^2
 * Range:     u8 n <= 15, u16 n <= 260, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 922 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Count the digit '5' in a number represented by seq_t */
static seq_t count_fives(seq_t v)
{
    seq_t cnt = 0;
    while (v > 0)
    {
        if ((v % 10) == 5)
            cnt++;
        v /= 10;
    }
    return cnt;
}

/* OEIS sequence A086013: number of 5's in decimal expansion of n^2 */
seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;   /* n^2 */
    return count_fives(sq);
}
