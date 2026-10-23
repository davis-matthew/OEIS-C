/*
 * A128315  Inverse Moebius transform of signed A007318.
 * https://oeis.org/A128315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the row‑wise entry T(r,k) of the triangle defined by
 *            T(r,k) = (-1)^k * Σ_{d|r} (-1)^d * binomial(d-1,k-1). Here r is
 *            the smallest integer with r(r+1)/2 ≥ n and k = n - (r-1)r/2
 *            (1‑based column)
 * Range:     i8 n <= 59, i16 n <= 179, i32 n <= 611, i64 n <= 2308,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static inline seq_t binom_simple(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        __int128 prod = (__int128)res * (n - k + i);
        prod /= i;
        res = (seq_t)prod;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* locate the triangle row r and column k for the nth term */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    __int128 prev_tri = (__int128)r * (r - 1) / 2;
    idx_t k = (idx_t)(n - (idx_t)prev_tri); /* 1‑based column */

    /* compute T(r,k) */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d) continue;
        if (d >= k) {
            seq_t bin = binom_simple(d - 1, k - 1);
            int sign_d = (d % 2 == 0) ? 1 : -1;
            sum += (seq_t)sign_d * bin;
        }
        idx_t d2 = r / d;
        if (d2 != d && d2 >= k) {
            seq_t bin = binom_simple(d2 - 1, k - 1);
            int sign_d = (d2 % 2 == 0) ? 1 : -1;
            sum += (seq_t)sign_d * bin;
        }
    }
    int sign_k = (k % 2 == 0) ? 1 : -1;
    return (seq_t)sign_k * sum;
}
