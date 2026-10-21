/*
 * A098353  Multiplication table of the odd numbers read by antidiagonals.
 * https://oeis.org/A098353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2i-1)*(2j-1) where (i,j) is the nth entry of the
 *            multiplication table of odd numbers read by antidiagonals
 * Range:     u8 n <= 142, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u128(__int128 v) {
    idx_t lo = 0;
    idx_t hi = (idx_t)1;

    /* find an upper bound for the sqrt */
    while (hi > 0 && ((__int128)hi * hi) <= v) {
        hi <<= 1;
        if (hi == 0) {
            hi = (idx_t)-1;
            break;
        }
    }
    lo = hi >> 1;                  /* floor(sqrt(v)) is between lo and hi-1 */

    /* binary search */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == v) return mid;
        if (sq < v) lo = mid + 1;
        else hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* Find the diagonal number s (>=2) such that n <= s*(s-1)/2. */
    __int128 val = (__int128)n * 8 + 1;    /* 1 + 8*n */
    idx_t sqrt_val = (idx_t)isqrt_u128(val);

    /* floor of (1 + sqrt)/2 */
    idx_t s0 = (sqrt_val + 1) >> 1;

    /* adjust to the smallest s satisfying n <= s*(s-1)/2 */
    seq_t check = ((seq_t)s0 * (seq_t)(s0 - 1)) / 2;
    idx_t s = (check < (seq_t)n) ? s0 + 1 : s0;

    /* number of terms before this diagonal */
    seq_t before = ((seq_t)(s - 1) * (seq_t)(s - 2)) / 2;

    idx_t k = (idx_t)((seq_t)n - before - 1);  /* 0‑based offset within diagonal */

    idx_t i = k + 1;                         /* row index (1‑based) */
    idx_t j = s - i;                         /* column index */

    seq_t odd_i = (seq_t)i * 2 - 1;
    seq_t odd_j = (seq_t)j * 2 - 1;
    return odd_i * odd_j;
}
