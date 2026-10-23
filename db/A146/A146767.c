/*
 * A146767  Triangle read by rows: expansion of p(x,n)=If[n == 0, 1, (x + 1)^n
 *          + 2^(n - 3)*Sum[Binomial[n, m]*x^m*(1 + x^(n - 2*m)), {m, 1, n -
 *          1}]].
 * https://oeis.org/A146767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) is the coefficient of x^k in p(x,r) = (x+1)^r + 2^(r-3) *
 *            Σ_{m=1}^{r-1} binom(r,m) * x^m * (1 + x^(r-2m)) where r is the
 *            row index (0-based) and k = n – r(r+1)/2. For 0 < k < r : a(n) =
 *            (1 + 2^(r-2)) * binom(r,k), otherwise 1
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  (row index) */
    idx_t low = 0, high = n + 1;           /* high is exclusive */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;                     /* final row index */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;               /* position in the row */

    /* Endpoints are always 1 */
    if (k == 0 || k == r || r <= 1)
        return (seq_t)1;

    /* Interior terms */
    seq_t pow = pow_uint(2, r - 2);         /* 2^(r-2) */
    seq_t coeff = binomial(r, k);          /* binom(r,k) */
    seq_t factor = pow + (seq_t)1;          /* 1 + 2^(r-2) */
    return coeff * factor;                 /* exact multiplication for seq_t */
}
