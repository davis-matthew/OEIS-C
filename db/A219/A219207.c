/*
 * A219207  Triangle, read by rows, where T(n,k) = binomial(n,k)^(k+1) for
 *          n>=0, k=0..n.
 * https://oeis.org/A219207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 40, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A219207
 *  Triangle read by rows, where T(n,k) = C(n,k)^(k+1)
 *  The sequence a(n) lists the triangle entries in row order.
 *
 *  For a given index n:
 *    * find r such that the first r rows contain ≤ n elements
 *      (i.e. r(r+1)/2 ≤ n < (r+1)(r+2)/2);
 *    * k = n - r(r+1)/2  (column within the row);
 *    * value = binomial(r,k)^(k+1)
 *  The functions binomial() and big_pow() are provided by the utilities.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r containing the term at index n.
     * We accumulate row sizes until the cumulative count exceeds n.
     *   cum128  : cumulative count (in 128‑bit to avoid overflow)
     *   r       : current row index
     */
    idx_t r = 0;
    unsigned __int128 cum128 = 0;

    while (cum128 + (unsigned __int128)(r + 1) <= n) {
        cum128 += (unsigned __int128)(r + 1);
        r++;
    }

    /* Elements before row r */
    idx_t cum  = (idx_t)cum128;
    /* Column within the row */
    idx_t k    = n - cum;

    /* Compute binomial coefficient and raise to power k+1 */
    seq_t base   = binomial(r, k);
    seq_t exponent = (seq_t)(k + 1);

    return big_pow(base, exponent);
}
