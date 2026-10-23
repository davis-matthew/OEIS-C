/*
 * A036710  a(n)=number of Gaussian integers z=a+bi satisfying
 *          n-1/2<|z|<=n+1/2, a>=0, b>=0.
 * https://oeis.org/A036710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of lattice points (a,b) with a,b>=0 such that *
 *            n^2-n+1 <= a^2+b^2 <= n^2+n. * This is computed by iterating
 *            over possible a and counting admissible b
 * Range:     u8 n <= 7, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 directly. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Compute the integer bounds for squared magnitude. */
    seq_t sq_n = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t lower = sq_n - (seq_t)n + (seq_t)1;  /* n^2 - n + 1 */
    seq_t upper = sq_n + (seq_t)n;             /* n^2 + n */

    seq_t count = (seq_t)0;

    /* Loop over possible a values. */
    for (idx_t a_val = (idx_t)0; ; ++a_val)
    {
        seq_t a_sq = (seq_t)a_val * (seq_t)a_val;

        /* If a^2 already exceeds upper bound, we are done. */
        if (a_sq > upper)
            break;

        /* Maximum b such that a^2 + b^2 <= upper */
        seq_t max_b_sq = upper - a_sq;
        seq_t max_b = floor_sqrt(max_b_sq);

        /* Minimum b such that a^2 + b^2 >= lower */
        seq_t min_b;
        if (a_sq >= lower) {
            min_b = (seq_t)0;
        } else {
            seq_t rem = lower - a_sq;          /* rem > 0 */
            seq_t t = floor_sqrt(rem);
            if (t * t < rem)  /* ceil of sqrt(rem) */
                ++t;
            min_b = t;
        }

        if (min_b > max_b)
            continue;   /* no admissible b for this a */

        count += (max_b - min_b) + (seq_t)1;
    }

    return count;
}
