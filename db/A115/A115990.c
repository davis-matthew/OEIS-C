/*
 * A115990  Riordan array (1/sqrt(1-2*x-3*x^2), (1-2*x-3*x^2)/(2*(1-3*x)) -
 *          sqrt(1-2*x-3*x^2)/2).
 * https://oeis.org/A115990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the flattened enumeration of the triangle T(n,k) =
 *            Σ_{j=k..n} C(n-k, j-k)*C(j, n-j) (OEIS A115990)
 * Range:     u8 n <= 27, u16 n <= 77, u32 n <= 275, u64 n <= 945,
 *            u128 n <= 3569, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Determine row r (0‑based) and column c for the flattened index n */
    idx_t t = n + 1;                                /* position in 1‑based count of entries */
    double d = sqrt((double)(8.0 * t + 1.0));       /* sqrt(8t+1) */
    idx_t r = (idx_t)((d - 3.0) / 2.0);             /* floor of solution */
    /* Fix possible rounding down */
    seq_t bound = ((seq_t)(r + 1) * (r + 2)) / 2;
    if (bound < t)
        ++r;
    idx_t prev_total = r * (r + 1) / 2;
    idx_t c = n - prev_total;                       /* column in row r */

    /* Compute T(r,c) */
    seq_t sum = 0;
    for (idx_t j = c; j <= r; ++j) {
        seq_t c1 = binomial(r - c, j - c);
        seq_t c2 = binomial(j, r - j);
        sum += c1 * c2;
    }
    return sum;
}
