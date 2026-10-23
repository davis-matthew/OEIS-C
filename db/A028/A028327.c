/*
 * A028327  Elements in the even-Pascal triangle A028326 that are not 2.
 * https://oeis.org/A028327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 141, u32 n <= 541, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028327
   Formula: a(n) = 2 * C(r, k) where
   r = smallest integer ≥ 2 such that r*(r-1)/2 > n
   k = n - (r-1)*(r-2)/2 + 1
   (This is the list of twice the interior binomial coefficients of Pascal's
    triangle, row by row.  The outer entries C(r,0) and C(r,r) are omitted
    because they would give the value 2, which is excluded.)

   The function implements that mapping and uses the helper
   `binom(idx_t a, idx_t b)` supplied by the build system.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binom.h"    /* declares seq_t binom(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r-1)/2 > n ≥ (r-1)*(r-2)/2. */
    /* Compute r using a double approximation, then correct by
       at most one step to avoid overflow and round‑off issues. */
    double d = sqrt(1.0 + 8.0 * (double)n);
    idx_t r = (idx_t)((1.0 + d) / 2.0);          /* floor of the root   */
    /* Adjust upward if the computed r does not satisfy the inequality. */
    while ((((unsigned __int128)r) * (r - 1) / 2) <= n)
        r++;

    /* Starting index of the row r (sum of terms from rows 2 … r‑1). */
    idx_t start = (idx_t)(((unsigned __int128)(r - 1) * (r - 2)) / 2);
    /* Position inside the row (0‑based). */
    idx_t offset = n - start;
    /* Binomial coefficient index k (1‑based, inside the row). */
    idx_t k = offset + 1;   /* 1 ≤ k ≤ r‑1 */

    /* The value is twice the binomial coefficient. */
    seq_t val = binom(r, k);
    val *= (seq_t)2;
    return val;
}
