/*
 * A185875  Third accumulation array of A051340, by antidiagonals.
 * https://oeis.org/A185875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the antidiagonal enumeration of the third accumulation
 *            array of A051340. The array entry T(n,k) that is used is T(n,k)
 *            = C(n+3,4)*C(k+2,3) - C(n+2,4)*C(k+1,3) for n,k ≥ 1. Here C(x,r)
 *            denotes the binomial coefficient (x choose r)
 * Range:     u8 n <= 18, u16 n <= 179, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 92 terms through n = 1256 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: binomial coefficient for r = 3 (returns 0 if x < 3). */
static inline uint64_t binom3_ull(idx_t x) {
    if (x < 3) return 0ULL;
    uint64_t u = (uint64_t)x;
    return u * (u - 1) * (u - 2) / 6ULL;
}

/* Helper: binomial coefficient for r = 4 (returns 0 if x < 4). */
static inline uint64_t binom4_ull(idx_t x) {
    if (x < 4) return 0ULL;
    uint64_t u = (uint64_t)x;
    return u * (u - 1) * (u - 2) * (u - 3) / 24ULL;
}

/* Find the antidiagonal index s such that (s-1)*s/2 >= n. */
static inline idx_t find_diagonal(idx_t n) {
    idx_t low = 2, high = 2;
    /* Grow high until it is large enough. */
    while ((unsigned __int128)(high - 1) * high / 2 < n) high <<= 1;
    /* Binary search. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)(mid - 1) * mid / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* Locate the (i,j) coordinates on the antidiagonal for a(n). */
    idx_t s = find_diagonal(n);                    /* s >= 2 */
    idx_t cumulative_before = (s - 2) * (s - 1) / 2;
    idx_t p = n - cumulative_before;               /* 1 <= p < s */
    idx_t i = p;                                   /* row index */
    idx_t j = s - p;                               /* column index */

    uint64_t t1 = binom4_ull(i + 3) * binom3_ull(j + 2);
    uint64_t t2 = binom4_ull(i + 2) * binom3_ull(j + 1);
    uint64_t result = t1 - t2;
    return (seq_t)result;
}
