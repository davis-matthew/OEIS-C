/*
 * A131129  3*A007318 - 2*A097806, where A007318 = Pascal's triangle and
 *          A097806 = the pairwise operator.
 * https://oeis.org/A131129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131129: 3*A007318 - 2*A097806, read by rows.
   For the n-th term, determine the triangle row r and column k
   (0‑based) such that
           T(r,k) = 3 * binom(r,k) - 2 * A097806(r,k),
   where A097806(r,k) = 1 if k >= r-1 (and hence always 1 for r=0,1),
   otherwise 0.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* find row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;               /* column 0‑based */

    /* binomial coefficient binom(r,k) */
    idx_t kk = k;
    if (kk > r - kk)
        kk = r - kk;                       /* use symmetry */

    seq_t comb = 1;
    for (idx_t i = 1; i <= kk; ++i) {
        comb = comb * (seq_t)(r - kk + i) / (seq_t)i;
    }

    /* subtract 2 only if k >= r-1 (for r>1) or always for r=0,1 */
    int subtract = (r <= 1) ? 1 : ((k >= r - 1) ? 1 : 0);

    return 3 * comb - 2 * (seq_t)subtract;
}
