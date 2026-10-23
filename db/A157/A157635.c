/*
 * A157635  Triangle read by rows: T(n,m) = 1 if n*m*(n-m) = 0, and n*m*(n-m)
 *          otherwise.
 * https://oeis.org/A157635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the linearized triangle defined by T(r,m) = 1 if r==0 ||
 *            m==0 || m==r = r * m * (r-m) otherwise where r = ⌊(√(8n+1)-1)/2⌋
 *            m = n - r(r+1)/2
 * Range:     u8 n <= 68, u16 n <= 2111, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit integers.
   Uses binary search with __uint128_t to avoid overflow in the product. */
static inline idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x, best = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x) return mid;
        if (sq < x) { best = mid; lo = mid + 1; }
        else        { hi = mid - 1; }
    }
    return best;
}

seq_t a(idx_t n) {
    /* compute row index r */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;

    /* linear index where the row r starts */
    idx_t offset = r * (r + 1) / 2;

    /* column index within the row */
    idx_t m = n - offset;

    /* triangle entry */
    if (r == 0 || m == 0 || m == r)
        return (seq_t)1;
    else
        return (seq_t)r * (seq_t)m * (seq_t)(r - m);
}
