/*
 * A103283  Triangle read by rows: T(n,k) is the coefficient of x^k in the
 *          monic characteristic polynomial of the n X n matrix with 2's on
 *          the diagonal and 1's elsewhere (n >= 1 and 0 <= k <= n). Row 0
 *          consists of the single term 1.
 * https://oeis.org/A103283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th term of the triangle read by rows T(r,k) =
 *            coefficient of x^k in the monic characteristic polynomial of the
 *            r×r matrix with 2's on the diagonal and 1's elsewhere. For r≥1:
 *            T(r,0) = (-1)^r ⋅ (r+1) T(r,r) = 1 T(r,k) = (-1)^{r-k} ⋅ [
 *            C(r-1,k-1) + (r+1)⋅C(r-1,k) ] (1 ≤ k < r) where C...
 * Range:     i8 n <= 30, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the helper function `binom(idx_t N, idx_t K)` from utils. */

#include <stdint.h>
#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the special first term */
    if (n == 0)
        return (seq_t)1;

    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    /* No backward adjustment needed because of the loop condition */

    idx_t prev_len = r * (r + 1) / 2;   /* number of terms before row r */
    idx_t k = n - prev_len;             /* column within the row (0 ≤ k ≤ r) */

    /* Special cases */
    if (k == r)                     /* leading coefficient is always 1 */
        return (seq_t)1;

    /* Sign factor: (-1)^(r-k) */
    seq_t sign = ((r - k) & 1ULL) ? (seq_t)-1 : (seq_t)1;

    if (k == 0)                     /* constant term */
    {
        seq_t val = sign * (seq_t)(r + 1);
        return val;
    }

    /* 1 ≤ k < r : use the explicit formula */
    seq_t b1 = binom(r - 1, k - 1);        /* C(r-1, k-1) */
    seq_t b2 = binom(r - 1, k);            /* C(r-1, k)   */
    seq_t prod = b2 * (seq_t)(r + 1); /* (r+1)*C(r-1,k) */
    seq_t sum = b1 + prod;
    return sign * sum;
}
