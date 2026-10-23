/*
 * A181656  Generalized (conditional) Riordan array with k-th column generated
 *          by x^k*(1+x^2) if k mod 2 = 0, x^k*(1+x) otherwise.
 * https://oeis.org/A181656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181656.  
   The entries are obtained by flattening the generalized Riordan array
   whose k‑th column has generating function

        g_k(x) = x^k*(1+x^2)   if k is even
                 x^k*(1+x)     if k is odd

   The n‑th term a(n) is the entry of this array in row‑major order:
   let r be the unique row with r(r+1)/2 ≤ n < (r+1)(r+2)/2
   and col = n – r(r+1)/2.  Then

        a(n) = 1  if r==col
             or (col even  and r==col+2)
             or (col odd   and r==col+1)
             else 0
*/

#include <math.h>

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((double)(8ull*n + 1)) - 1) / 2);

    /* adjust r if the floating point estimate was off by one */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;

    /* starting index of this row */
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t col = (idx_t)(n - start);

    /* compute the entry value */
    if (r == col)            return (seq_t)1;
    else if ((col & 1) == 0) { /* even column */
        if (r == col + 2)     return (seq_t)1;
    } else { /* odd column */
        if (r == col + 1)     return (seq_t)1;
    }

    return (seq_t)0;
}
