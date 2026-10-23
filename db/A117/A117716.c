/*
 * A117716  Triangle T(n,k) read by rows: the coefficient [x^n] of
 *          x^2/(1-(k+1)*x-x^3) in row n, columns 0 <= k <= n.
 * https://oeis.org/A117716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 41, u32 n <= 76, u64 n <= 185,
 *            u128 n <= 460, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A117716 is the concatenation of the rows of the
   triangle T(n,k) defined by the coefficients of the generating
   function

        x^2 / (1 - (k+1)x - x^3)

   In row n (0‑based) there are n+1 entries, indexed by 0 ≤ k ≤ n.
   For a linear index `i` (our function argument `n`) we first find
   the row `r` such that
        r*(r+1)/2  ≤ i < (r+1)*(r+2)/2
   and then the column `k = i - r*(r+1)/2`.  The value
   T(r,k) satisfies the recurrence

        a_0 = 0,  a_1 = 0,  a_2 = 1
        a_m = (k+1)*a_{m-1} + a_{m-3}   for m ≥ 3

   The function below implements this definition directly, computing
   only the needed row.  All arithmetic uses `seq_t` for the value
   type and `idx_t` for indices, as required. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r and column k from the linear index n. */
    idx_t r = 0;
    /* r is the largest integer with r*(r+1)/2 ≤ n.  The loop
       runs at most O(sqrt(2n)) times, which is fine for the
       expected limits (≈140 for n≈1e4). */
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;   /* column index within the row */

    /* Compute T(r,k) by the recurrence.  The first three terms are
       independent of k. */
    if (r == 0) return (seq_t)0;   /* a_0 */
    if (r == 1) return (seq_t)0;   /* a_1 */
    if (r == 2) return (seq_t)1;   /* a_2 */

    seq_t prev3 = (seq_t)0;  /* a_0 */
    seq_t prev2 = (seq_t)0;  /* a_1 */
    seq_t prev1 = (seq_t)1;  /* a_2 */
    seq_t cur   = (seq_t)0;
    seq_t k1    = (seq_t)(k + 1);  /* (k+1) as seq_t */

    for (idx_t m = 3; m <= r; ++m) {
        cur   = k1 * prev1 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
