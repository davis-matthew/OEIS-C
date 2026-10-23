/*
 * A194293  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=n, r=(1+sqrt(5))/2,
 *          the golden ratio.
 * https://oeis.org/A194293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = g(r,k) where r = minimal integer with r(r+1)/2 ≥ n, k = n
 *            – r(r−1)/2, and g(r,k) = |{ i : 1 ≤ i ≤ r and floor( r·{i·φ} ) =
 *            k−1 }|, φ = (1+√5)/2. The function computes the flattened values
 *            of the triangular array defined in A194293. All arithmetic is
 *            integer‐only; φ is approximated by a 128...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t isqrt128(unsigned __int128 v)
{
    uint64_t lo = 0;
    uint64_t hi = UINT64_MAX;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* φ = (1+√5)/2 approximated as N/(2D).
       Choose D = 2^60;  error < 5·10^‑15, sufficient for n ≤ 10⁴. */
    static uint64_t N = 0, denom = 0;
    static unsigned char inited = 0;
    if (!inited) {
        const uint64_t D = (uint64_t)1 << 60;                     // 2^60
        unsigned __int128 temp = (unsigned __int128)5 * D * D;    // 5·D² < 2¹²⁸
        uint64_t sqrt5D = isqrt128(temp);                        // ⌊√5·D⌋
        N = D + sqrt5D;                                           // numerator
        denom = 2 * D;                                            // denominator
        inited = 1;
    }

    /* Find the row r (1‑based) and the column k (1‑based) that contain index n. */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;
    idx_t prev = (idx_t)((unsigned __int128)(r - 1) * r / 2);
    idx_t k = n - prev;   /* column index in this row */

    /* Count the i with floor(r*{i·φ}) = k−1. */
    idx_t count = 0;
    for (idx_t i = 1; i <= r; ++i) {
        unsigned __int128 prod = (unsigned __int128)i * N;
        idx_t floor_i = (idx_t)(prod / denom);
        uint64_t rem = (uint64_t)(prod % denom);   /* numerator of fractional part */
        idx_t bin = (idx_t)(( (unsigned __int128)r * rem ) / denom) + 1;
        if (bin == k) ++count;
    }

    return (seq_t)count;
}
