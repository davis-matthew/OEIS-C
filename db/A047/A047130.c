/*
 * A047130  Array read by descending antidiagonals: T(h,k) is the number of
 *          paths consisting of steps from (0,0) to (h,k) such that each step
 *          has length 1 directed up or right and no up-step crosses the line
 *          y = 3x/4. (Thus a path crosses the line only at lattice points and
 *          on right-steps.)
 * https://oeis.org/A047130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(h,k) where T(h,k) counts lattice paths * from (0,0)
 *            to (h,k) using right ( +1,0 ) and up ( 0,+1 ) steps, * subject
 *            to the restriction that no up step crosses the line y = 3x/4. *
 *            Equivalently, a path is valid iff for every up step from (x,
 *            y-1) to (x, y) * we have (4*y <= 3*x) OR (4*(y...
 * Range:     u8 n <= 81, u16 n <= 237, u32 n <= 718, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the antidiagonal (s) that contains n.  The total
     * number of elements up to and including row s is
     * total(s) = (s+1)(s+2)/2.
     */
    idx_t s = 0;
    while ( ((s + 1) * (s + 2) / 2) <= n ) {
        s++;
    }

    /* Compute offset within this antidiagonal and obtain
     * the (h,k) coordinates.
     */
    idx_t prev_total = s * (s + 1) / 2;   /* total(s-1) */
    idx_t offset     = n - prev_total;    /* offset in [0, s] */
    idx_t h = s - offset;                 /* row index */
    idx_t k = offset;                     /* column index */

    /* Allocate a 2D DP table of size (s+1) x (s+1).  The size is
     * small (≈140×140 for n≈10000) so this per-call allocation is fine.
     */
    size_t size = (size_t)(s + 1) * (s + 1);
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* return 0 on allocation failure */

    /* Compute T(h,k) for all 0 <= i,j <= s. */
    for (idx_t i = 0; i <= s; ++i) {
        for (idx_t j = 0; j <= s; ++j) {
            size_t idx = (size_t)i * (s + 1) + j;
            if (i == 0 && j == 0) {
                dp[idx] = (seq_t)1;
            } else {
                seq_t val = 0;
                if (j > 0) {
                    /* Right step from (i, j-1) to (i, j). Always allowed. */
                    val += dp[(size_t)i * (s + 1) + (j - 1)];
                }
                if (i > 0) {
                    /* Up step from (i-1, j) to (i, j) may be forbidden. */
                    /* Condition: 4*i <= 3*j  OR  4*i >= 3*j + 4. */
                    /*   4*i <= 3*j   ->  up step ends at or below the line. */
                    /*   4*i >= 3*j+4 ->  up step starts at or above the line. */
                    if ( ( (4 * i <= 3 * j) || (4 * i >= 3 * j + 4) ) ) {
                        val += dp[(size_t)(i - 1) * (s + 1) + j];
                    }
                }
                dp[idx] = val;
            }
        }
    }

    seq_t result = dp[(size_t)h * (s + 1) + k];
    free(dp);
    return result;
}
