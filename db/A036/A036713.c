/*
 * A036713  a(n)=number of Gaussian integers z=a+bi satisfying |z|<=n+1/2,
 *          a>=0, 0<=b<a.
 * https://oeis.org/A036713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by counting Gaussian integers z = a+bi with |z| <= n+1/2,
   a >= 0 and 0 <= b < a.  The count is obtained by iterating over all
   admissible a and, for each, determining the largest b that satisfies
   a² + b² <= n² + n. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Radius squared (R = n + 1/2) expressed as an integer inequality:
       a² + b² <= n² + n   because (n+1/2)² = n² + n + 1/4. */
    seq_t limit = (seq_t)n * ((seq_t)n + 1);

    seq_t count = 0;

    /* Maximal integer a such that a² <= limit. */
    idx_t max_a = (idx_t)floor_sqrt(limit);

    for (idx_t a_val = 1; a_val <= max_a; ++a_val)
    {
        seq_t a_sq = (seq_t)a_val * (seq_t)a_val;
        seq_t diff = limit - a_sq;           /* >= 0 */

        /* Largest integer b with b² <= diff. */
        seq_t bmax_full = floor_sqrt(diff);
        idx_t bmax = (idx_t)bmax_full;

        /* Enforce the constraint b < a. */
        if (bmax >= a_val)
            bmax = a_val - 1;

        /* Count b = 0 .. bmax inclusive. */
        count += (seq_t)bmax + (seq_t)1;
    }

    return count;
}
