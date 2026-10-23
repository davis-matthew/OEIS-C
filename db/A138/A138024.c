/*
 * A138024  Triangle read by rows: T(n,k) = (n!/2) * [t^n*x^k] (1 -
 *          exp(2*x*t)*(t - 1)/(1 + t)), 0 <= k <= n.
 * https://oeis.org/A138024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  less, tabl, sign
 * Formula:   a(n) = T(r,k), where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2.
 *            T(r,r) = 2^(r-1) (T(0,0)=1), T(r,k) = (-1)^(r-k) * (r! / k!) *
 *            2^k for 0 ≤ k < r
 * Range:     i8 n <= 15, i16 n <= 35, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = (idx_t)((sqrtl(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    /* Adjust r in case of rounding errors. */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    while (r * (r + 1) / 2 > n)
        --r;

    idx_t k = n - r * (r + 1) / 2;

    /* Handle the diagonal elements. */
    if (k == r) {
        if (r == 0)
            return (seq_t)1;          /* T(0,0) = 1 */
        return pow2(r - 1);          /* 2^(r-1) for r ≥ 1 */
    }

    /* Compute T(r,k) for k < r:
       val = (r! / k!) * 2^k with alternating sign (-1)^(r-k). */
    seq_t val = (seq_t)1;
    for (idx_t i = k + 1; i <= r; ++i)
        val = val * (seq_t)i;        /* multiply by i for i = k+1 … r */

    val = val * pow2(k);             /* multiply by 2^k */

    /* Apply the sign factor (-1)^(r-k). */
    if ((r - k) & 1)
        val = -val;

    return val;
}
