/*
 * A180445  All positive solutions, x, for each of two Diophantine equations
 *          noted by Richard K. Guy.
 * https://oeis.org/A180445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the nth positive integer x obtained from solutions of
 *            2^(z+3) – 7 = s², with x = (s + 1)/2. Only five such solutions
 *            exist
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   floor_sqrt, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_seq.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n must be at least 1.  For n == 0 return 0. */
    if (n == 0)
        return 0;

    idx_t count = 0;
    /* All solutions occur for z = 0,1,2,4,12, corresponding
       to 2^(z+3) up to 2^15.  A modest bound of 30 is more than
       enough to find every solution and keeps the routine
       very fast. */
    const idx_t MAX_Z = 30;
    for (idx_t z = 0; z <= MAX_Z; ++z)
    {
        /* Compute 2^(z+3) */
        seq_t pow2 = pow_seq(2, z + 3);

        /* s² = pow2 – 7  (pow2 ≥ 8, so this is non‑negative) */
        seq_t s_sq = pow2 - 7;

        /* Integer square root of s_sq */
        seq_t s = floor_sqrt(s_sq);

        /* Check that s² equals s_sq and that s is odd */
        if (s * s == s_sq && (s & (seq_t)1))
        {
            /* Corresponding x = (s + 1)/2 */
            seq_t x = (s + 1) / 2;
            ++count;
            if (count == n)
                return x;
        }
    }
    /* No further solutions exist; return 0 for n > 5. */
    return 0;
}
