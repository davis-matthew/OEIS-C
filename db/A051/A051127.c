/*
 * A051127  Table T(n,k) = k mod n read by antidiagonals (n >= 1, k >= 1).
 * https://oeis.org/A051127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   linearize T(n,k)=k mod n by antidiagonals. For a given index n,
 *            find the diagonal d such that d(d+1)/2 ≥ n, then determine the
 *            row r and column k within that diagonal, and return k mod r
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned __int128 number (returns floor). */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 63;          /* sqrt(2^127) < 2^63 */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety, though OEIS offset starts at 1 */

    /* Find diagonal d: smallest d with d*(d+1)/2 >= n. */
    unsigned __int128 eight_n = (unsigned __int128)8 * n + 1;
    uint64_t s = isqrt_uint128(eight_n);          /* floor(sqrt(8n+1)) */
    uint64_t t = (s - 1) >> 1;                    /* floor((sqrt(8n+1)-1)/2) */
    unsigned __int128 sum_t = (unsigned __int128)t * (t + 1) / 2;
    uint64_t d = (sum_t < n) ? t + 1 : t;

    /* Position within diagonal: 0-based offset */
    unsigned __int128 prev_sum = (unsigned __int128)(d - 1) * d / 2;
    uint64_t offset = (uint64_t)(n - prev_sum - 1);

    /* Compute coordinates (row r, column k) */
    uint64_t r = offset + 1;            /* row index (n) */
    uint64_t k = d - offset;            /* column index (k) */

    /* Return value k mod r */
    return (seq_t)(k % r);
}
