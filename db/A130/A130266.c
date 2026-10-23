/*
 * A130266  A051340 * A128174.
 * https://oeis.org/A130266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is read from a lower‑triangular matrix T(r,k) where r =
 *            floor((sqrt(8*n+1)-1)/2) (row), k = n - r(r+1)/2 (column). For
 *            each entry one has let m = r - k (= r-k); if (m is even) T(r,k)
 *            = (r+1) + m/2; else T(r,k) = (m+1)/2. Using this rule the first
 *            few terms are 1,1,2,4,1,3,2,5,1,4,7,2,6,1,5,3...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard types */
#include <stdint.h>

/* Integer square‑root: floor(sqrt(x)) for 0 ≤ x < 2^64.
   Uses a binary search with 128‑bit multiplication to avoid overflow. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = 0xFFFFFFFFULL; /* 2^32-1, its square fits in 64 bits */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t s = isqrt_u64((unsigned __int128)8 * n + 1);
    idx_t r = (s - 1) / 2;

    /* Compute the starting index of row r */
    idx_t offset = (unsigned __int128)r * (r + 1) / 2;

    /* Column index k (0‑based) */
    idx_t k = n - offset;

    /* m = r - k = distance of the column from the right end */
    idx_t m = r - k;

    /* Apply the formula for the triangle entry */
    seq_t result;
    if ((m & 1ULL) == 0) {                  /* m even  */
        result = (seq_t)(r + 1) + (seq_t)(m >> 1);
    } else {                                 /* m odd   */
        result = (seq_t)((m + 1) >> 1);
    }
    return result;
}
