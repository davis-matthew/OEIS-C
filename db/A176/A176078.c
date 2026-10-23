/*
 * A176078  Triangle, read by rows, T(n, k) = (2*n)!/((n-k)! * k!)^2 -
 *          (2*n)!/(n!)^2 + 1.
 * https://oeis.org/A176078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binomial(2*n,n)*( binomial(n,k)^2 - 1) + 1 a(n) is the
 *            triangle read in row‑major order
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 58, u64 n <= 179,
 *            u128 n <= 643, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* integer floor square root for idx_t (uint64_t) */
static idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        /* mid = floor((lo + hi) / 2) */
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t mid_sq = mid * mid;
        if (mid_sq <= x) {
            /* same check to avoid overflow when mid == x */
            idx_t next = mid + 1;
            if (next <= x && next * next <= x) {
                lo = next;
            } else {
                return mid;
            }
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t sq = isqrt_u64(1 + 8 * n);      /* sqrt(1+8n) */
    idx_t r  = (sq - 1) / 2;              /* floor((-1+sqrt(...))/2) */
    idx_t offset = r * (r + 1) / 2;       /* starting index of row r */
    idx_t k = n - offset;                 /* column within the row */

    /* Compute T(r,k) */
    seq_t bin2n_n = binom(2 * r, r);
    seq_t bin_n_k = binom(r, k);
    seq_t val = bin2n_n * (bin_n_k * bin_n_k - 1) + 1;
    return val;
}
