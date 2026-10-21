/*
 * A073216  The terms of A055235 (sums of two powers of 3) divided by 2.
 * https://oeis.org/A073216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A073216 (sum of two powers of 3 divided by 2).
   The sequence is the list of values (3^k + 3^l) / 2 for all 0 ≤ l ≤ k,
   sorted in nondecreasing order.  For a given 0‑based index n the associated
   pair (k,l) satisfies
        n < (k+1)(k+2)/2   and   n ≥ k(k+1)/2,
   so l = n – k(k+1)/2.
*/
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find the row k such that n lies in its interval. */
    idx_t k = 0;
    idx_t rem = n;
    while (rem >= k + 1) {
        rem -= k + 1;
        ++k;
    }
    idx_t l = rem;            /* column index within the row */

    seq_t pk = pow3(k);
    seq_t pl = pow3(l);
    return (pk + pl) / 2;
}
