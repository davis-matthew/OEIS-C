/*
 * A054450  Triangle of partial row sums of unsigned triangle A049310(n,m), n
 *          >= m >= 0 (Chebyshev S-polynomials).
 * https://oeis.org/A054450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth element (row‑major) of the triangle T(n, m)
 *            where T(n, m) = sum_{s=0..floor((n‑m)/2)} F_{n‑m‑2s+1} *
 *            binomial(m + s – 1, s), F_k is the kth Fibonacci number (F_0 =
 *            0, F_1 = 1). Indexing is 0‑based
 * Range:     u8 n <= 90, u16 n <= 299, u32 n <= 1127, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = 1ULL << 32;  /* safe upper bound for 64‑bit sqrt */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;  /* avoid infinite loop */
            high = mid - 1;
        }
    }
    return ans;
}

static inline seq_t fib(idx_t k)
{
    if (k == 0)
        return (seq_t)0;
    if (k == 1)
        return (seq_t)1;
    seq_t a = (seq_t)0, b = (seq_t)1, tmp;
    for (idx_t i = 2; i <= k; ++i) {
        tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t index)
{
    /* Find row r such that r(r+1)/2 <= index < (r+1)(r+2)/2 */
    idx_t val = 8 * index + 1;
    idx_t s = isqrt_u64(val);
    idx_t r = (s - 1) / 2;
    /* adjust in case of rounding errors */
    while (r * (r + 1) / 2 > index) --r;
    while ((r + 1) * (r + 2) / 2 <= index) ++r;
    idx_t col = index - r * (r + 1) / 2;

    idx_t n = r, m = col;
    seq_t sum = (seq_t)0;

    if (m == 0) {
        sum = fib(n + 1);
    } else {
        idx_t max_s = (n - m) / 2;
        for (idx_t s = 0; s <= max_s; ++s) {
            idx_t fib_idx = n - m - 2 * s + 1;   /* >= 1 */
            seq_t f_val = fib(fib_idx);
            seq_t b_val = binom(m + s - 1, s);
            sum += f_val * b_val;
        }
    }
    return sum;
}
