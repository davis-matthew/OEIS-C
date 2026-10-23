/*
 * A124428  Triangle, read by rows: T(n,k) =
 *          binomial(floor(n/2),k)*binomial(floor((n+1)/2),k).
 * https://oeis.org/A124428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(floor(r/2),k) * binomial(floor((r+1)/2),k) where
 *            r is the row such that the total number of terms up to row r–1
 *            is ≤ n < the total up to row r, and k = n – (total up to row
 *            r–1)
 * Range:     u8 n <= 44, u16 n <= 124, u32 n <= 369, u64 n <= 1276,
 *            u128 n <= 2600, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t cumulative = 0;   /* number of terms seen so far   */
    idx_t r = 0;            /* current row index              */
    idx_t k;

    /* Find the row that contains the n-th term. */
    for (;;) {
        idx_t len = (r / 2) + 1;          /* number of terms in row r */
        if (n < cumulative + len) {
            k = n - cumulative;          /* column index within row r */
            break;
        }
        cumulative += len;
        ++r;
    }

    /* Compute T(r,k) = binomial(floor(r/2),k) * binomial(floor((r+1)/2),k). */
    seq_t left  = binomial(r / 2, k);
    seq_t right = binomial((r + 1) / 2, k);
    return left * right;
}
