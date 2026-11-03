/*
 * A086076  Number of 5's in decimal expansion of triangular number n(n+1)/2.
 * https://oeis.org/A086076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal 5‑digits in n(n+1)/2
 * Range:     u8 n <= 24, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* Compute the triangular number using seq_t arithmetic.   */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    /* Count the occurrences of the digit '5' in its decimal   */
    /* representation. The helper returns a seq_t value.      */
    return count_fives(tri);
}
