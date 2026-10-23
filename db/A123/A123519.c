/*
 * A123519  Triangle read by rows: T(n,k) number of tilings of a 2n X 3 grid
 *          by dominoes, 2k of which are in a vertical position (0<=k<=n).
 * https://oeis.org/A123519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^k * binomial(r + k, 2*k) where r and k satisfy
 *            offset_row(r) = r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n -
 *            offset_row(r)
 * Range:     u8 n <= 22, u16 n <= 59, u32 n <= 198, u64 n <= 683,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* -------------------------- integer utilities -------------------------- */

static idx_t isqrt_uint128(__int128 x)
{
    idx_t low = 0, high = (idx_t)1 << 63, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* --------------------------- triangle helpers --------------------------- */

static void row_k_from_index(idx_t n, idx_t *row, idx_t *k)
{
    if (n == 0) { *row = 0; *k = 0; return; }
    /* Solve r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       r is floor((sqrt(8*n+1)-1)/2). */
    __int128 tmp = (__int128)8 * n + 1;
    idx_t sq = isqrt_uint128(tmp);
    idx_t r = (sq - 1) / 2;

    /* adjust in case of rounding error */
    __int128 offset = (__int128)r * (r + 1) / 2;
    if (offset > n) {
        while (offset > n) {
            r--;
            offset = (__int128)r * (r + 1) / 2;
        }
    } else {
        while (1) {
            __int128 next = (__int128)(r + 1) * (r + 2) / 2;
            if (next > n) break;
            r++;
            offset = next;
        }
    }
    *row = r;
    *k = n - (idx_t)offset;
}

/* ------------------------ binomial coefficient ------------------------ */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* ---------------------- main sequence function ----------------------- */

seq_t a(idx_t n)
{
    idx_t r, k;
    row_k_from_index(n, &r, &k);

    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) pow2 = pow2 + pow2;   // multiply by 2

    seq_t bin = binom(r + k, 2 * k);
    return bin * pow2;
}
