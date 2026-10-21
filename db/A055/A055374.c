/*
 * A055374  Invert transform applied three times to Pascal's triangle A007318.
 * https://oeis.org/A055374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th entry obtained by reading the triangle T(n,k) =
 *            4^(n-1) * C(n,k) (for n>0) row‑wise from the top. a(0) = 1.
 *            T(0,0) is also 1
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* sqrtl */

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* first term */

    /* Find the row r (0‑based) such that
          r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    uint64_t low  = 0;
    uint64_t high = (uint64_t)(sqrtl((long double)n * 2.0L)) + 2;
    while (low <= high) {
        uint64_t mid = low + (high - low) / 2;
        __int128  s = (__int128)mid * (mid + 1) / 2;
        if (s <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    uint64_t r = high;               /* row index (0‑based) */
    __int128 prev = (__int128)r * (r + 1) / 2;
    idx_t k = (idx_t)(n - prev);     /* column index within the row */

    /* For r==0 the entry is 1, already handled by the n==0 case. */
    if (r == 0) return 1;

    /* Compute binomial coefficient C(r,k).  Use the smaller of k and r‑k. */
    idx_t kk = k;
    if (kk > r - kk) kk = r - kk;
    seq_t binom = 1;
    for (idx_t i = 1; i <= kk; ++i) {
        binom = binom * (seq_t)(r - kk + i) / (seq_t)i;
    }

    /* Compute 4^(r-1) by repeated multiplication. */
    seq_t pow4 = 1;
    for (idx_t i = 1; i < r; ++i) {
        pow4 *= 4;
    }

    return binom * pow4;
}
