/*
 * A193902  Triangular array: the self-fusion of (p(n,x)), where
 *          p(n,x)=2x*p(n-1,x)+1, p(0,x)=1.
 * https://oeis.org/A193902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = let r = floor((sqrt(8n+1)-1)/2); // row index with
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2 let k = n - r*(r+1)/2; // column
 *            within the row if r == 0 : a(n) = 1 else if k == r : a(n) = 2^r
 *            - 1 else : a(n) = 2^(r+1) - 2^(r-k) (0 <= k < r)
 * Range:     u8 n <= 35, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t triangular_root(idx_t n)
{
    /* Find the greatest r such that r*(r+1)/2 <= n */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = triangular_root(n);
    if (r == 0) {
        return pow2(0);          /* a(0) = 1 */
    }

    /* starting index of row r */
    idx_t offset = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - offset;       /* column index (0‑based) */

    if (k == r) {
        seq_t val = pow2(r);    /* 2^r - 1 */
        --val;
        return val;
    } else {
        seq_t val = pow2(r + 1);
        val -= pow2(r - k);     /* 2^(r+1) - 2^(r-k) */
        return val;
    }
}
