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
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* sum C(M, j) for j = 0..t  (t ≤ M) */
static seq_t binom_cumulative(idx_t M, idx_t t)
{
        seq_t sum = 1;          /* C(M,0) */
        seq_t C   = 1;
        for (idx_t j = 1; j <= t; ++j) {
                C = C * (seq_t)(M - j + 1) / (seq_t)j;
                sum += C;
        }
        return sum;
}

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
        seq_t C   = binomial(M, r - k);          /* C = C(M, r-k)  */
        seq_t first  = (seq_t)(k - 1) * C;    /* (k-1)*C >= 0   */
        seq_t second = (seq_t)(k - 2) * S;    /* (k-2)*S >= 0   */
        return first - second;                /* subtraction is safe because first ≥ second  */
}
