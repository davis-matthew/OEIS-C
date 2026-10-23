/*
 * A035628  Number of partitions of n into parts 5k and 5k+2 with at least one
 *          part of each type.
 * https://oeis.org/A035628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 117, u32 n <= 332, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula implemented: a(n) = number of partitions of n using parts
   5k (k>=1) and 5k+2 (k>=0) that contain at least one part of each type.
   Computed as:
       a(n) = p(n) - p0(n) - p2(n)
   where p(n) is partitions using all allowed parts,
         p0(n) partitions using only multiples of 5,
         p2(n) partitions using only numbers congruent to 2 (mod 5).
   The partitions are counted without regard to order.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; n=0 is unused, return 0 */
        return (seq_t)0;

    /* Allocate temporary arrays for dynamic programming */
    seq_t *dps  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp0  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp2  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));

    if (!dps || !dp0 || !dp2)   /* allocation failure, return 0 */
    {
        free(dps); free(dp0); free(dp2);
        return (seq_t)0;
    }

    /* Base case: one way to partition 0 */
    dps[0] = dp0[0] = dp2[0] = (seq_t)1;

    /* ---- partitions using all allowed parts ---- */
    /* parts of the form 5k (k>=1) */
    for (idx_t part = 5; part <= n; part += 5)
        for (idx_t i = part; i <= n; ++i)
            dps[i] += dps[i - part];

    /* parts of the form 5k+2 (k>=0) */
    for (idx_t part = 2; part <= n; part += 5)
        for (idx_t i = part; i <= n; ++i)
            dps[i] += dps[i - part];

    /* ---- partitions using only multiples of 5 ---- */
    for (idx_t part = 5; part <= n; part += 5)
        for (idx_t i = part; i <= n; ++i)
            dp0[i] += dp0[i - part];

    /* ---- partitions using only numbers 5k+2 ---- */
    for (idx_t part = 2; part <= n; part += 5)
        for (idx_t i = part; i <= n; ++i)
            dp2[i] += dp2[i - part];

    /* Compute the desired value */
    seq_t result = dps[n];
    result -= dp0[n];
    result -= dp2[n];

    /* Free temporary memory */
    free(dps);
    free(dp0);
    free(dp2);

    return result;
}
