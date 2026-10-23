/*
 * A199011  Triangle T(n,k), read by rows, given by
 *          (1,1,-1,1,0,0,0,0,0,0,0,...) DELTA (0,1,0,0,0,0,0,0,0,0,0,0,...)
 *          where DELTA is the operator defined in A084938.
 * https://oeis.org/A199011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) enumerates the triangle T(n,k)=binomial(n,k+1) in row‑major
 *            order. For n>0 let r be the unique integer with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 and set c = n – r(r+1)/2. Then a(n) = binom(r,
 *            c+1). For n==0, a(0)=1
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t r   = 0;   /* current row index   */
    idx_t tri = 0;   /* start index of row r */

    /* Move to the row that contains index n. The loop is O(√n). */
    while (tri + (r + 1) <= n) {
        r++;
        tri += r;
    }

    idx_t c = n - tri;          /* column index within the row */
    return binom(r, c + 1);     /* T(r,c) = binomial(r,c+1)  */
}
