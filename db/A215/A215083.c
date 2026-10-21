/*
 * A215083  Triangle T(n,k) = sum of the k first n-th powers.
 * https://oeis.org/A215083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{j=1}^{r} j^k, where * k = floor((sqrt(8n+1)-1)/2), *
 *            r = n - k(k+1)/2. * (Equivalent: find the unique k with k(k+1)/2
 *            <= n < (k+1)(k+2)/2, * then a(n) = sum of the first r = n -
 *            k(k+1)/2 integers raised to the k‑th power.)
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 63, u64 n <= 151,
 *            u128 n <= 404, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find k such that k*(k+1)/2 <= n < (k+1)*(k+2)/2   */
    idx_t low  = 0;
    idx_t high = n;          /* upper bound – safe for any n >= 0   */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* Use 128‑bit arithmetic to avoid overflow */
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t k = low;
    /* t_k = k(k+1)/2 */
    idx_t t_k = (idx_t)(((__uint128_t)k * (k + 1)) >> 1);
    idx_t r   = n - t_k;            /* 0 <= r <= k */

    seq_t sum = 0;
    for (idx_t j = 1; j <= r; ++j) {
        sum += big_pow((seq_t)j, (seq_t)k);
    }
    return sum;
}
