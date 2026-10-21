/*
 * A212385  Number of Dyck n-paths all of whose ascents have lengths equal to
 *          1 (mod 5).
 * https://oeis.org/A212385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0}^{n} binom(4*k-3*n-1 , n-k) * *
 *            binom(n+1 , 5*k-4*n)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 53, u128 n <= 100,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation evaluates the sum by cancelling the common
 * divisor (n+1) term‑wise.  This keeps intermediate values bounded
 * and works correctly for all supplied seq_t types. */

#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Offset 0: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Lower bound for k where the second binomial is non‑zero:
     *   k >= ceil(4*n/5) = (4*n + 4) / 5                     */
    idx_t kmin = (4*n + 4) / 5;
    idx_t kmax = n;

    seq_t sum   = 0;
    seq_t denom = (seq_t)(n + 1);   /* (n+1) is the outer divisor   */

    for (idx_t k = kmin; k <= kmax; ++k) {
        /*----- first binomial: binom(4*k-3*n-1 , n-k) -----*/
        idx_t top1    = 4*k - 3*n - 1;
        idx_t bottom1 = n - k;
        if (bottom1 > top1)   /* binom would be zero */
            continue;

        seq_t b1 = binom(top1, bottom1);

        /*----- second binomial: binom(n+1 , 5*k-4*n) -----*/
        idx_t bottom2 = 5*k - 4*n;            /* always >= 0 for k >= kmin */
        seq_t b2 = binom(n + 1, bottom2);

        /* Cancel the common divisor (n+1) before forming the product.
         * After this cancellation each term equals
         *     b1 * b2 / (n+1)   and fits in seq_t.             */
        seq_t g = gcd(b1, denom);
        seq_t b1r = b1 / g;
        seq_t denom1 = denom / g;

        g = gcd(b2, denom1);
        seq_t b2r = b2 / g;
        /* denom1 / g should be 1 (since (n+1) divides b1*b2).   */

        seq_t term = b1r * b2r;    /* Safe for all seq_t sizes          */
        sum += term;
    }

    return sum;   /* division by (n+1) has been performed term‑wise   */
}
