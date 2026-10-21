/*
 * A061885  n + largest triangular number less than or equal to n.
 * https://oeis.org/A061885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n + largest triangular number ≤ n
 *
 * For a given n, the largest triangular number T_k = k*(k+1)/2
 * satisfying T_k ≤ n is obtained from the positive root of
 * k^2 + k - 2n ≤ 0:
 *     k = floor( (sqrt(1+8n) - 1) / 2 ).
 * The returned value is (seq_t)n + T_k.
 */

#include <stdint.h>

/* 128‑bit integer square root: floor(sqrt(x)) for x ≤ 2^128-1 */
static seq_t
isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = (unsigned __int128)1 << 64; /* sqrt(2^128-1) < 2^64 */
    unsigned __int128 ans = 0;

    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (seq_t)ans;
}

seq_t
a(idx_t n)
{
    /* compute k = floor((sqrt(1+8n)-1)/2) using 128‑bit arithmetic */
    unsigned __int128 val = (unsigned __int128)1 + (unsigned __int128)8 * n;
    seq_t k = (isqrt_u128(val) - 1) / 2;

    /* triangular number T_k = k*(k+1)/2 */
    seq_t triangular = k * (k + 1) / 2;

    /* result a(n) = n + T_k */
    return (seq_t)n + triangular;
}
