/*
 * A124725  Triangle read by rows: T(n,k) = binomial(n,k) + binomial(n,k+2) (0
 *          <= k <= n).
 * https://oeis.org/A124725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binomial(n,k) + binomial(n,k+2) (0 <= k <= n), write
 *            the triangle by rows. a(n) returns the nth element of the
 *            concatenation of all rows of T. For each n the algorithm finds
 *            the row r containing the element and the column k, then returns
 *            binomial(r,k)+binomial(r,k+2)
 * Range:     u8 n <= 37, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t lo = 0, hi = n, r = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    idx_t prev_total = r * (r + 1) / 2;
    idx_t k = n - prev_total;

    seq_t val = binom_idx(r, k);
    if (k + 2 <= r) val += binom_idx(r, k + 2);
    return val;
}
