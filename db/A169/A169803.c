/*
 * A169803  Triangle read by rows: T(n,k) = binomial(n+1-k,k) (n >= 0, 0 <= k
 *          <= n).
 * https://oeis.org/A169803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2) and k = n -
 *            r*(r+1)/2; T(r,k) = binomial(r+1‑k, k). This is the triangle
 *            read by rows definition from OEIS A169803
 * Range:     u8 n <= 108, u16 n <= 357, u32 n <= 1287, u64 n <= 4777,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 64‑bit integer. */
static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        /* use the 128‑bit type to prevent overflow in mid*mid */
        if ((__uint128_t)mid * mid <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Multiplicative binomial coefficient C(n,k) returning a seq_t value.
   The product is performed in 128‑bit to avoid overflow before the
   division. */
static inline seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;   /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res * (n - k + i) may exceed 64 bits, use 128 bits */
        res = (seq_t)(((__uint128_t)res * (n - k + i)) / i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Row number r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = (_isqrt_u64(8 * n + 1) - 1) / 2;

    /* Position k within that row (0‑based). */
    idx_t k = n - r * (r + 1) / 2;

    return _binom(r + 1 - k, k);
}
