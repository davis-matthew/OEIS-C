/*
 * A144389  Triangle T(n,k) = n*binomial(n - 1, k) - (-1)^(n - k)*binomial(n,
 *          k), T(0,0) = 1, read by rows, 0 <= k <= n.
 * https://oeis.org/A144389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = the sequence obtained by reading the triangle * T(n,k)
 *            row‑by‑row, where T(n,k) = n*binomial(n-1,k) -
 *            (-1)^(n-k)*binomial(n,k) * for a(0) we have T(0,0) = -1
 * Range:     i8 n <= 7, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
    /* Solve r^2 + r - 2n <= 0   →   r = floor((sqrt(1+8n)-1)/2)   */
    seq_t sq = floor_sqrt((seq_t)(1 + 8 * n));
    idx_t r = ((idx_t)sq - 1) / 2;
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Special case for the very first element (n = 0) */
    if (r == 0 && k == 0)
        return -1;

    /* Compute T(r,k) */
    seq_t binom1 = binomial(r - 1, k);  /* binomial(r-1,k)  */
    seq_t binom2 = binomial(r,     k);  /* binomial(r,k)    */

    seq_t term = (seq_t)r * binom1;     /* r * binomial(r-1,k) */
    if ((r - k) & 1)                     /* (r-k) odd → sign = -1 */
        term += binom2;                 /* add binom2      */
    else
        term -= binom2;                 /* subtract binom2 */

    return term;
}
