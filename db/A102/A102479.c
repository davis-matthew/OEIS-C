/*
 * A102479  Triangle read by rows: row n contains the numbers C(n,k)^(k-1) for
 *          0 <= k <= n-1, n >= 1.
 * https://oeis.org/A102479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n‑th entry of the triangle read by rows. Row r
 *            (1‑based) contains the numbers C(r, k)^(k-1) for 0 ≤ k < r
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* find smallest r such that r(r+1)/2 >= n */
    idx_t low = 1, high = n;      /* upper bound is safe for n up to 10⁴ */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* previous cumulative term count */
    __uint128_t prev = (__uint128_t)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev - 1);   /* 0‑based index within the row */

    if (k == 0)          /* C(r,0)^(‑1) == 1 */
        return (seq_t)1;
    if (k == 1)          /* C(r,1)^(0) == 1 */
        return (seq_t)1;

    seq_t coef = binom(r, k);          /* C(r,k) */
    seq_t result = seq_pow(coef, k - 1);   /* (C(r,k))^(k-1) */
    return result;
}
