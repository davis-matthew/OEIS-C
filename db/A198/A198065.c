/*
 * A198065  Triangle read by rows (n >= 0, 0 <= k <= n, m = 5); T(n,k) =
 *          Sum{j=0..m} Sum{i=0..m} (-1)^(j+i)*C(i,j)*n^j*k^(m-j).
 * https://oeis.org/A198065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle * T(r,k) = 3*r*k^4 -
 *            6*k^3*r^2 + 7*k^2*r^3 - 4*k*r^4 + r^5 * where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r(r+1)/2
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) for 64‑bit x */
static inline idx_t isqrt_u64(idx_t x) {
    idx_t low = 0, high = 0xFFFFFFFFULL, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { ans = 0; low = 1; continue; }
        if (mid <= x / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (isqrt_u64((idx_t)(8ULL * n + 1)) - 1) >> 1;
    /* adjust r in case of rounding errors (safe for all n) */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t k = n - r * (r + 1) / 2;

    /* cast indices to seq_t for arithmetic */
    seq_t rs = (seq_t)r;
    seq_t ks = (seq_t)k;

    /* powers of r */
    seq_t rs2 = rs * rs;
    seq_t rs3 = rs2 * rs;
    seq_t rs4 = rs3 * rs;
    seq_t rs5 = rs4 * rs;

    /* powers of k */
    seq_t ks2 = ks * ks;
    seq_t ks3 = ks2 * ks;
    seq_t ks4 = ks3 * ks;

    /* compute the formula */
    seq_t term1 = (seq_t)3 * rs * ks4;
    seq_t term2 = (seq_t)6 * ks3 * rs2;
    seq_t term3 = (seq_t)7 * ks2 * rs3;
    seq_t term4 = (seq_t)4 * ks * rs4;

    seq_t result = term1 - term2 + term3 - term4 + rs5;
    return result;
}
