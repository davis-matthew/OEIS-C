/*
 * A235706  (I + A132440)^3: Coefficients for normalized generalized Laguerre
 *          polynomials n!*Lag(n, 3-n, -x).
 * https://oeis.org/A235706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 941, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235706: coefficients of the normalized generalized Laguerre
   polynomials n!*Lag(n,3-n,-x).
   The values are those of the triangle
              T(i,j) = binomial(3,i-j) * i! / j!, 0 ≤ j ≤ i
   listed row‑by‑row, i.e.
          a(n) = T(row, k)
   where `row` and `k` are obtained from the row‑major index `n` by
   solving row*(row+1)/2 ≤ n < (row+1)*(row+2)/2.
   A simple integer square‑root is used to find `row`. */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {
            if (n == 0) return 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {          /* mid*mid ≤ n  */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* binomial coefficients for (3 choose d), d = 0..3 */
static const seq_t BINOM3[4] = {1, 3, 3, 1};

seq_t a(idx_t n)
{
    /* row = floor((sqrt(8n+1)-1)/2) */
    idx_t discr = (idx_t)8 * n + (idx_t)1;
    idx_t r     = isqrt_u64(discr);
    idx_t row   = (r - (idx_t)1) / (idx_t)2;

    /* first index of this row in row‑major order */
    idx_t first = row * (row + (idx_t)1) / (idx_t)2;
    idx_t k     = n - first;             /* column index within row */

    /* check whether the entry lies within the 4‑by‑row band 0 ≤ row-k ≤ 3 */
    idx_t d = 0;
    if (row >= k) d = row - k;
    if (d > (idx_t)3) return (seq_t)0;

    /* compute T(row,k) = binom(3,row-k) * row! / k!  */
    seq_t res = BINOM3[d];
    for (idx_t i = k + (idx_t)1; i <= row; ++i)
        res = res * (seq_t)i;

    return res;
}
