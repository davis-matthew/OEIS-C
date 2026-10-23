/*
 * A028930  Theta series of quadratic form (or lattice) with Gram matrix [ 4,
 *          1; 1, 6 ].
 * https://oeis.org/A028930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y)∈ℤ² : 2x² + x y + 3y² = n}|
 * Range:     u8 n <= 2499, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* count solutions using a direct, bounded exhaustive search          */
    seq_t count = 0;

    /* Upper bounds for |x| and |y| derived from the minimum of the form   */
    /* For fixed y: minimal value occurs at x = –y/4, giving (23/8)·y² */
    /* => 23 y² ≤ 8 n  →  |y| ≤ floor(√(8 n/23))                     */
    /* For fixed x: minimal value occurs at y = –x/6, giving (23/12)·x² */
    /* => 23 x² ≤ 12 n →  |x| ≤ floor(√(12 n/23))                     */

    idx_t xlim = 0;
    while ((seq_t)23 * (seq_t)xlim * (seq_t)xlim <= (seq_t)12 * (seq_t)n)
        xlim++;
    if (xlim > 0) xlim--;   /* last value that satisfies the inequality */

    idx_t ylim = 0;
    while ((seq_t)23 * (seq_t)ylim * (seq_t)ylim <= (seq_t)8 * (seq_t)n)
        ylim++;
    if (ylim > 0) ylim--;   /* last value that satisfies the inequality */

    for (long long sx = -(long long)xlim; sx <= (long long)xlim; ++sx)
    {
        for (long long sy = -(long long)ylim; sy <= (long long)ylim; ++sy)
        {
            long long val = 2LL * sx * sx + sx * sy + 3LL * sy * sy;
            if (val == (long long)n)
                count++;
        }
    }

    return count;
}
