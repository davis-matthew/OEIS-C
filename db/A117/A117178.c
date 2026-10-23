/*
 * A117178  Riordan array (c(x^2)/sqrt(1-4*x^2), x*c(x^2)), c(x) the g.f. of
 *          A000108.
 * https://oeis.org/A117178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 170, u32 n <= 594, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A117178.  The terms are the entries of the Riordan array
   T(n,k) read row by row (row 0, then row 1, …).  For a given index
   n:
     • find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2,
       i.e. r is the largest row whose starting index is ≤ n.
     • let c = n - r(r+1)/2;  // column within the row
     • set d = r - c.  If d is odd the entry is 0.
     • if d is even let k = d/2; then a(n) = binomial(r+1, k).
   The helper `binomial` comes from the provisioned utilities.
*/

#include <stdint.h>
#include "../utils/binomial.h"   /* supplies seq_t binomial(idx_t n, idx_t k) */

static idx_t _row_of(idx_t n)
{
    idx_t low = 0, high = n, best = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            best = mid;
            low  = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* determine row and column */
    idx_t r = _row_of(n);
    unsigned __int128 tri_r = (unsigned __int128)r * (r + 1) / 2;
    idx_t col = n - (idx_t)tri_r;

    /* check parity of r - col */
    idx_t d = r - col;
    if (d & 1U)            /* odd -> entry is zero */
        return (seq_t)0;

    idx_t k = d >> 1;      /* d/2 */
    /* compute the binomial coefficient */
    return binomial(r + 1, k);
}
