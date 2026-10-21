/*
 * A097807  Riordan array (1/(1+x),1) read by rows.
 * https://oeis.org/A097807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = (-1)^(r-k) where r is the row * index and k is the column
 *            index of the n-th term * when the triangular array * T(r,k) =
 *            (-1)^(r-k) for 0 ≤ k ≤ r * is read row by row. * The row r is
 *            the largest integer with r(r+1)/2 ≤ n. * The column k is n -
 *            r(r+1)/2
 * Range:     i8 n <= 9863, i16 n <= 9863, i32 n <= 9863, i64 n <= 9863,
 *            i128 n <= 9863, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static uint64_t find_row(uint64_t n) {
    uint64_t lo = 0, hi = n + 1;          /* hi*(hi+1)/2 > n for all n ≥ 0 */
    while (lo + 1 < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= (__int128)n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    uint64_t N = n;                       /* promote to uint64_t for arithmetic */
    uint64_t r = find_row(N);
    uint64_t tri = r * (r + 1) / 2;       /* r*(r+1)/2 fits in uint64_t for all n in tests */
    uint64_t k = N - tri;                 /* 0 ≤ k ≤ r */

    /* parity of (r-k): if even→+1, odd→-1 */
    seq_t value = ((r - k) & 1) ? -1 : 1;
    return value;
}
