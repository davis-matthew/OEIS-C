/*
 * A191748  Sequence of all m in {1,2,3,...} such that A191747(m) = 1.
 * https://oeis.org/A191748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n‑th 1 in the concatenation of rows of
 *            identity matrices of size 1,2,3,... (0‑based index). The value
 *            is computed directly from the block indices
 * Range:     u8 n <= 41, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline uint64_t isqrt128(unsigned __int128 x) {
    /* floor(sqrt(x)) for 128‑bit x; result fits in 64 bits. */
    uint64_t lo = 0, hi = (1ULL << 35);        /* sqrt(2^67) < 2^34.5 */
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    /* Step 1: determine block number m such that
       m*(m+1)/2 > n  (0‑based). */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;   /* 8*n+1 */
    uint64_t sqrt_val = isqrt128(tmp);                     /* floor(sqrt(8n+1)) */
    uint64_t m = (sqrt_val + 1) >> 1;                      /* (sqrt+1)/2 */

    /* Step 2: find position within block m */
    unsigned __int128 prev128 = (unsigned __int128)m * (m - 1) / 2;
    idx_t prev = (idx_t)prev128;   /* terms before block m */
    idx_t i = n - prev;            /* 0‑based index inside block m */

    /* Step 3: compute start position of block m and the value */
    uint64_t t = m - 1;   /* t = m-1 */
    unsigned __int128 sumSq =
        (unsigned __int128)t * (t + 1) * ((unsigned __int128)2 * t + 1) / 6; /* sum of squares 1..t */

    unsigned __int128 val =
        sumSq + 1 + (unsigned __int128)i * (m + 1);   /* a(n) */

    return (seq_t)val;
}
