/*
 * A176639  Triangle T(n, k) = 15^(k*(n-k)), read by rows.
 * https://oeis.org/A176639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 15^(k*(n-k)), where k is the position of the n-th term in
 *            its row of the triangle T(r, k) = 15^(k*(r-k)), read by rows.
 *            Row r (0‑based) contains r+1 entries: T(r,0)…T(r,r)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 23, u64 n <= 47, u128 n <= 82,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square root of a 64‑bit integer (floor). */
static inline idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        /* guard against overflow of mid*mid by using 128‑bit unsigned integer */
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* External function required by the system. */
/* We include the header that declares pow_uint.  The header name follows the
   naming convention described in the documentation. */
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
       By solving r^2 + r - 2n ≤ 0 we get r = floor((sqrt(8n+1)-1)/2). */
    idx_t r = (isqrt_idx(8 * n + 1) - 1) / 2;

    /* Column index k within the row. */
    idx_t offset = r * (r + 1) / 2;          /* index where row r starts */
    idx_t k = n - offset;                    /* 0 ≤ k ≤ r */

    /* Exponent e = k*(r-k). This fits in idx_t for the test limits. */
    idx_t e = k * (r - k);

    /* Compute 15^e exactly.  pow_uint(base,exp) handles large exponents
       by repeated multiplication.  seq_t is large enough for the result
       in the tested ranges. */
    return pow_uint(15ull, e);
}
