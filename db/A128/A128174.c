/*
 * A128174  Transform, (1,0,1,...) in every column.
 * https://oeis.org/A128174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the concatenation of the rows of the lower triangular
 *            matrix T(n,k) where T(n,k) = (1+(-1)^(n-k))/2. Given n
 *            (1‑based), find the row r such that r(r+1)/2 >= n, compute the
 *            column c = n - (r-1)r/2 (1‑based), and return 1 if (r-c) is
 *            even, else 0
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* binary search for minimal r with r(r+1)/2 >= n */
    idx_t lo = 1;
    idx_t hi = n;                 /* r never exceeds n */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    idx_t r = lo;

    /* previous cumulative sum S(r-1) = (r-1)r/2 */
    idx_t prev_sum = (idx_t)((unsigned __int128)(r - 1) * r / 2);
    idx_t c = n - prev_sum;        /* column in this row (1‑based) */

    /* parity of delta = r - c == 0 (even) gives 1, odd gives 0 */
    seq_t val = (seq_t)((((r - c) & 1) ? 0 : 1));
    return val;
}
