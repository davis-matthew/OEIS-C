/*
 * A212213  Array read by antidiagonals: pi(n) + pi(k) - pi(n+k), where pi() =
 *          A000720.
 * https://oeis.org/A212213
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   not stated
 * Range:     u8 n <= 4951, u16 n <= 4951, u32 n <= 4951, u64 n <= 4951,
 *            u128 n <= 4951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212213
   a(n) = pi(i) + pi(j) - pi(i + j)
   where (i,j) are obtained by reading the two‑dimensional array
   A(i,j) = pi(i)+pi(j)-pi(i+j) (with i,j ≥ 2) along its antidiagonals,
   starting with a(2) = A(2,2).  The mapping from the 1‑based index n
   to the coordinates (i,j) is given by the algorithm below.
*/

#include <stdint.h>
#include <stdbool.h>

static seq_t compute_pi(idx_t n)
{
    if (n < 2) return 0;
    seq_t cnt = 0;
    for (idx_t x = 2; x <= n; ++x)
    {
        bool prime = true;
        for (idx_t d = 2; d <= x / d; ++d)
        {
            if (x % d == 0) { prime = false; break; }
        }
        if (prime) ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below the offset are undefined */

    /* zero‑based position in the enumeration that starts at a(2) = A(2,2) */
    idx_t t = n - 2;

    /* find the diagonal index diag such that diag*(diag+1)/2 <= t < (diag+1)*(diag+2)/2 */
    idx_t diag = 0;
    while ((diag + 1) * (diag + 2) / 2 <= t) ++diag;

    /* number of terms preceding this diagonal */
    idx_t prev = diag * (diag + 1) / 2;

    /* position within the current diagonal (0‑based) */
    idx_t offset = t - prev;

    /* sum of the coordinates on this diagonal: i + j = 4 + diag */
    idx_t sum = 4 + diag;

    /* convert offset to coordinates
       i increases from 2 upwards; j is determined by the sum
    */
    idx_t i = 2 + offset;      /* 1‑based after offset */
    idx_t j = sum - i;         /* j >= 2 automatically */

    seq_t pi_i   = compute_pi(i);
    seq_t pi_j   = compute_pi(j);
    seq_t pi_sum = compute_pi(i + j);

    return pi_i + pi_j - pi_sum;
}
