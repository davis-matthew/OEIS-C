/*
 * A073254  Array read by antidiagonals, A(n,k) = n^2 + n*k + k^2.
 * https://oeis.org/A073254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = n^2 + n*k + k^2 read by antidiagonals where the pair
 *            (n,k) is determined from the linear index. The mapping is: index
 *            i → diagonal r such that r*(r+1)/2 ≤ i < (r+1)*(r+2)/2, then
 *            within that diagonal let n = i - r*(r+1)/2, and k = r - n. So
 *            a(i) = n^2 + n*k + k^2
 * Range:     u8 n <= 135, u16 n <= 9810, u32 n <= 9810, u64 n <= 9810,
 *            u128 n <= 9810, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find diagonal index r such that C(r) = r*(r+1)/2 ≤ n < C(r+1). */
    idx_t lo = 0;
    idx_t hi = 1;
    while ((__int128)hi * (hi + 1) / 2 <= n) {
        if (hi > (1ULL << 63)) break;   /* prevent overflow */
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;
    idx_t prev = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t pos = n - prev;              /* position within diagonal, 0..r */
    idx_t nn = pos;
    idx_t kk = r - pos;

    seq_t sn = (seq_t)nn;
    seq_t sk = (seq_t)kk;
    seq_t val = sn * sn + sn * sk + sk * sk;
    return val;
}
