/*
 * A123588  Triangle read by rows: T(n, k) is the coefficient of x^k in the
 *          polynomial 1 - ChT(n, x^(1/2))^2, where ChT(n, x) is the n-th
 *          Chebyshev polynomial of the first kind, evaluated at x (0 <= k <=
 *          n).
 * https://oeis.org/A123588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n, k) = (-1)^(n + k - 1) * 4^(k - 1) * (2*binomial(n + k, 2*k)
 *            - binomial(n + k - 1, 2*k - 1)), T(n, 0) = n mod 2. The sequence
 *            a(n) is read row‑wise from the triangle T(n, k), with a(0) =
 *            T(0, 0)
 * Range:     i8 n <= 12, i16 n <= 39, i32 n <= 113, i64 n <= 394,
 *            i128 n <= 1274, big any n
 * Helpers:   binomial, pow4_const
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow4_const.h"

/* Integer square root for an unsigned 64‑bit integer.
   The function returns floor(sqrt(x)). */
static inline idx_t
isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, mid;
    idx_t res = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq == (__int128)x) {
            res = mid;
            break;
        }
        if (sq < (__int128)x) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Locate the row r and column c such that
       r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t root = isqrt_u64(8 * n + 1);
    idx_t r = (root - 1) >> 1;
    idx_t tri_r = r * (r + 1) / 2;
    idx_t c = n - tri_r;               /* column index (0 <= c <= r) */

    /* Handle the diagonal entry T(r,0). */
    if (c == 0) return (seq_t)(r & 1);

    /* Compute the remaining coefficient T(r,c). */
    seq_t bin1 = binomial(r + c, 2 * c);
    seq_t bin2 = binomial(r + c - 1, 2 * c - 1);
    seq_t val = bin1 * (seq_t)2 - bin2;      /* inner bracket */

    seq_t pow4 = pow4_const(c - 1);
    seq_t res  = val * pow4;

    /* Apply the sign factor (-1)^(r + c - 1). */
    if (((r + c - 1) & 1) != 0) res = -res;

    return res;
}
