/*
 * A221877  Triangle read by rows: T(n,k) = number of order-preserving or
 *          order-reversing full contraction mappings (of an n-chain) with
 *          height exactly k.
 * https://oeis.org/A221877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th entry of the triangle * T(r,k) read by rows,
 *            where * T(r,1) = r * T(r,k) = 2·(r-k+1)·C(r-1,k-1) for k > 1 *
 *            The index n is 1‑based
 * Range:     u8 n <= 31, u16 n <= 125, u32 n <= 478, u64 n <= 1980,
 *            u128 n <= 7935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root: floor(sqrt(x)) using 128‑bit arithmetic */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 32);           /* 2^32 > sqrt(2^64) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* multiplicative binomial coefficient: C(n,k) */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for 0‑index, not used */

    /* find row r such that r(r+1)/2 >= n */
    idx_t r = (floor_sqrt_u64(8 * n + 1) - 1) / 2;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* column k within the row */
    idx_t prev_sum = ((r - 1) * r) / 2;
    idx_t k = n - prev_sum;

    if (k == 1)
        return (seq_t)r;

    seq_t bin = binom_idx(r - 1, k - 1);
    seq_t res = ((seq_t)2) * (seq_t)(r - k + 1) * bin;
    return res;
}
