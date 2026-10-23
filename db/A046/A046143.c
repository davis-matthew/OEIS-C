/*
 * A046143  Triangle T(n, k) = gcd(2^n-1, 2^k-1), for n>=1 and 1 <= k <= n.
 * https://oeis.org/A046143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^gcd(r,k) - 1, where r is the row index with r(r+1)/2 ≥
 *            n and k = n - r(r-1)/2 is the column index in that row
 * Range:     u8 n <= 44, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute a(n) from the flattened triangle of T(r,k) = gcd(2^r-1, 2^k-1) */
seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while ((__int128)r * (r + 1) / 2 < n) {
        r++;
    }
    /* Compute the column k */
    idx_t prev_tri = (__int128)(r - 1) * r / 2;  /* (r-1)*r/2 */
    idx_t k = n - prev_tri;

    /* Compute gcd(r,k) and raise 2 to that power */
    idx_t g = gcd_idx(r, k);
    seq_t val = pow2(g);
    return val - (seq_t)1;
}
