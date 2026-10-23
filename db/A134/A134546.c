/*
 * A134546  Triangle read by rows: T(n, k) = Sum_{j=0..n} floor(j / k).
 * https://oeis.org/A134546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where r = smallest integer with r(r+1)/2 ≥ n, c =
 *            n - (r-1)r/2, and T(r,c) = k*floor(r/k)*(floor(r/k)-1)/2 + (r
 *            mod k + 1)*floor(r/k) (k=c)
 * Range:     u8 n <= 253, u16 n <= 8900, u32 n <= 8900, u64 n <= 8900,
 *            u128 n <= 8900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline uint64_t isqrt_u128(__int128 x) {
    uint64_t r = (uint64_t)floorl(sqrtl((long double)x));
    while ((__int128)(r + 1) * (r + 1) <= x) r++;
    while ((__int128)r * r > x) r--;
    return r;
}

seq_t a(idx_t n) {
    /* Determine the row r and column c of the n-th element in the triangle. */
    uint64_t idx = (uint64_t)n;            /* n fits in uint64_t */
    /* Find r such that r*(r+1)/2 >= idx */
    __int128 val = (__int128)1 + (__int128)8 * idx;
    uint64_t sqrt_val = isqrt_u128(val);
    uint64_t r = (sqrt_val - 1) / 2;
    while ((__int128)r * (r + 1) / 2 < idx) r++;

    uint64_t prev_sum = (uint64_t)r * (r - 1) / 2; /* sum of lengths of previous rows */
    uint64_t c = idx - prev_sum;                   /* 1‑based column index */

    /* Compute T(r,c) using the closed formula. */
    uint64_t m = r / c;
    uint64_t r_mod = r % c;

    seq_t k   = (seq_t)c;
    seq_t m_s = (seq_t)m;
    seq_t r_mod_s = (seq_t)r_mod;

    seq_t prod = (m_s - 1) * m_s / 2;          /* (m-1)*m/2 */
    seq_t sum1 = prod * k;                     /* k*(m-1)*m/2 */
    seq_t sum2 = (r_mod_s + 1) * m_s;          /* (r mod k + 1)*m */
    return sum1 + sum2;
}
