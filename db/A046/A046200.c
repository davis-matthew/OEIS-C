/*
 * A046200  Odd numbers in the triangle of denominators in Leibniz's Harmonic
 *          Triangle.
 * https://oeis.org/A046200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n-th odd denominator in the triangle of denominators of Leibniz's
 *  Harmonic Triangle.  The triangle has entries 1/(n*C(n-1,k-1)).  We list all
 *  odd values of the denominator n*C(n-1,k-1) row by row (rows are numbered
 *  starting at 1).  Only odd rows produce odd denominators, because
 *  n must be odd and C(n-1,k) must be odd.  For an odd row n, the number of
 *  odd terms in that row equals 2^{popcount(n-1)}.  Each odd term is
 *  n*C(n-1,k) where C(n-1,k) is odd.  The sequence is obtained by
 *  concatenating these values in row-major order.  This implementation
 *  finds the row containing the desired index and then extracts the
 *  appropriate value.  Binomial coefficients are obtained from the
 *  provided binom() helper.  The implementation works for any seq_t /
 *  idx_t defined by the build. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* target index (1‑based) */
    idx_t target = n;
    idx_t cum = 0;                    /* cumulative count of odd terms found so far */

    /* Iterate over odd rows only (row 1,3,5,…) */
    for (idx_t row = 1; ; row += 2) {
        idx_t r = row - 1;                 /* n-1 for this row */

        /* Number of odd terms in this row: 2^{popcount(n-1)} */
        int pop = __builtin_popcountll(r);
        idx_t odd_in_row;
        if (pop >= 63)                        /* too many, we only need to know it's >= target */
            odd_in_row = target;
        else
            odd_in_row = ((idx_t)1) << pop;

        /* Check if the desired term lies in this row */
        if (cum + odd_in_row >= target) {
            idx_t offset = target - cum - 1;   /* 0‑based position within the row */

            /* Scan k = 0 … r to find the offset‑th odd binomial coefficient */
            for (idx_t k = 0; k <= r; ++k) {
                /* C(r,k) is odd iff k is a subset of r's bits */
                if ((k & (~r)) == 0) {
                    if (offset == 0) {
                        /* Compute the odd denominator: n * C(r, k) */
                        seq_t val = (seq_t)row * binom(r, k);
                        return val;
                    }
                    --offset;
                }
            }
            /* Should never reach here */
            return (seq_t)0;
        }

        cum += odd_in_row;   /* move to the next odd row */
    }
}
