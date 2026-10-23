/*
 * A108579  Number of symmetry classes of 3 X 3 magic squares (with distinct
 *          positive entries) having magic sum 3n.
 * https://oeis.org/A108579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1/4)*(n-2)^2)-floor((1/3)*(n-1))
 * Range:     u8 n <= 17, u16 n <= 514, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 1 or 2 the formula would involve a negative square
       if evaluated with unsigned arithmetic. Handle those cases
       explicitly. */
    if (n <= 2)
        return 0;

    /* Use signed arithmetic for the (n-2) value to avoid
       underflow when n = 1. The range of n in typical use
       (up to ~10⁴) is far below INT64_MAX, so the multiplication
       is safe. */
    int64_t s = (int64_t)n - 2;      /* s = n - 2 */
    seq_t sq = (seq_t)(s * s);       /* (n-2)^2 */
    seq_t part1 = sq / 4;            /* floor((n-2)^2 / 4) */
    seq_t part2 = ((seq_t)n - 1) / 3;/* floor((n-1) / 3)   */
    return part1 - part2;
}
