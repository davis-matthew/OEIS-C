/*
 * A130128  Triangle read by rows: T(n,k) = (n - k + 1)*2^(k-1).
 * https://oeis.org/A130128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, walk
 * Formula:   a(n) is the nth term of the triangle read by rows where T(n,k) =
 *            (n-k+1)*2^(k-1). Given n we locate the row r such that r*(r+1)/2
 *            >= n, find k = n - r*(r-1)/2 (1 ≤ k ≤ r) and return
 *            (r-k+1)*2^(k-1)
 * Range:     u8 n <= 43, u16 n <= 151, u32 n <= 559, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static inline idx_t isqrt_uint128(unsigned __int128 x) {
    /* Binary search for floor(sqrt(x)).  x fits in 128 bits. */
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64; /* hi > sqrt(max) */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq < x) {
            if (mid + 1 > hi) break;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid;
        }
    }
    if ((lo * lo) > x) lo -= 1;
    return (idx_t)lo;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* out‑of‑offset guard */

    /* Find the row r.  r·(r+1)/2 ≥ n. */
    unsigned __int128 disc = (unsigned __int128)8 * n + 1;
    idx_t sqrt_disc = isqrt_uint128(disc);
    idx_t r = (sqrt_disc - 1) / 2;      /* initial approximation */

    /* Correct in case the floor sqrt caused r to be too small. */
    if (((unsigned __int128)r * (r + 1)) / 2 < n) ++r;

    /* Number of terms preceding row r. */
    idx_t prev_total = (idx_t)(((unsigned __int128)r * (r - 1)) / 2);

    /* Column index k (1‑based). */
    idx_t k = n - prev_total;           /* 1 ≤ k ≤ r */

    /* Compute (r-k+1)*2^(k-1). */
    seq_t factor = (seq_t)(r - k + 1);
    seq_t pow    = pow2(k - 1);          /* pre‑computed power of two */
    return factor * pow;
}
