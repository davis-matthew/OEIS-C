/*
 * A029595  Theta series of quadratic form (or lattice) with Gram matrix [ 1,
 *          0, 0; 0, 2, 1; 0, 1, 3 ].
 * https://oeis.org/A029595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x1,x2,x3) to x1^2 + 2*x2^2 +
 *            2*x2*x3 + 3*x3^2 = n
 * Range:     u8 n <= 35, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function iterates over all integer pairs (x2,x3) whose quadratic
   expression is ≤ n and counts the number of possible x1 values
   (one solution if the remainder is zero, two otherwise). */

# include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Result counter */
    seq_t count = 0;

    /* Bound for |x2| and |x3|: safely larger than necessary. */
    seq_t bound_seq = floor_sqrt((seq_t)n) + (seq_t)1;
    long long bound = (long long)bound_seq;   /* signed limit for looping */

    for (long long x3 = -bound; x3 <= bound; ++x3)
    {
        for (long long x2 = -bound; x2 <= bound; ++x2)
        {
            /* Compute 2*x2^2 + 2*x2*x3 + 3*x3^2 */
            seq_t m = (seq_t)2 * (seq_t)(x2 * x2)
                    + (seq_t)2 * (seq_t)(x2 * x3)
                    + (seq_t)3 * (seq_t)(x3 * x3);

            if (m > n) continue;          /* skip if value exceeds n */

            seq_t r = n - m;              /* remainder that must be a perfect square */

            seq_t s = floor_sqrt(r);      /* integer square root of r */
            if (s * s == r)
            {
                count += (r == 0) ? (seq_t)1 : (seq_t)2;
            }
        }
    }

    return count;
}
