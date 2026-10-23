/*
 * A107030  Number triangle associated with the Riordan arrays
 *          (1/(1+x),x/(1+x)^k),k>=0.
 * https://oeis.org/A107030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n-th element in row-major order of the triangle *
 *            T(r,k) defined by * T(r,k) = (k-1)*C(k*(r-k), r-k) -
 *            (k-2)*sum_{j=0..r-k} C(k*(r-k), j) * with C(m,t) binomial
 *            coefficient, r=k..n, k≥0. * * a(n) = T(r, k) where r = row_of(n)
 *            (the unique r with r(r+1)/2 ≤ n < (r+1)(r+2)/2) * and k = n...
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom, binom_cumulative, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/row_of.h"
#include "../utils/binom_cumulative.h"

seq_t a(idx_t n)
{
        /* Locate the row and column of the flattened index n */
        idx_t r = row_of(n);   /* r(r+1)/2 ≤ n < (r+1)(r+2)/2   */
        idx_t base = (idx_t)r * ((idx_t)r + 1) / 2;   /* starting index of row r */
        idx_t k = n - base;                 /* column within the row (0‑based) */

        /* Special case: the very first element */
        if (r == 0 && k == 0) return 1;

        /* Pre‑compute M = k*(r-k) and the cumulative sum S = Σ_{j=0}^{r-k} C(M, j) */
        idx_t M = (idx_t)k * (r - k);       /* M ≤ 4900 for n ≤ 10000 */
        idx_t t = r - k;                    /* t ≤ r ≤ 141 for n ≤ 10000 */
        seq_t S  = binom_cumulative(M, t);

        if (k == 0) {           /* T(r,0) = 2 * S  for r > 0 */
                return 2 * S;
        }
        if (k == 1) {           /* T(r,1) = S */
                return S;
        }

        /* General case (k ≥ 2) */
        seq_t C   = binom(M, r - k);          /* C = C(M, r-k)  */
        seq_t first  = (seq_t)(k - 1) * C;    /* (k-1)*C >= 0   */
        seq_t second = (seq_t)(k - 2) * S;    /* (k-2)*S >= 0   */
        return first - second;                /* subtraction is safe because first ≥ second  */
}
