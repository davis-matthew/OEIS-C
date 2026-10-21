/*
 * A095836  Triangle read by rows: T(n,k) = k^((n-k)^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) with T(n,k)=k^((n-k)^2). r = smallest integer with
 *            r(r+1)/2 ≥ n (row index) k = n - (r-1)r/2 (position in row) Then
 *            a(n) = k^((r-k)^2)
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 29, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* Find an upper bound for the row */
    while (hi * (hi + 1) / 2 < n)
        hi <<= 1;
    /* Binary search the smallest r with r(r+1)/2 ≥ n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Integer exponentiation by squaring: computes b^e (e ≥ 0). */
static seq_t pow_seq_t(seq_t b, idx_t e)
{
    seq_t r = (seq_t)1;
    while (e > 0) {
        if (e & 1)
            r *= b;
        if (e > 1)
            b *= b;
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1)  /* Undefined for n=0, but caller guarantees n≥1 */
        return (seq_t)0;

    idx_t r = find_row(n);
    idx_t prev_start = (r - 1) * r / 2;
    idx_t k = n - prev_start;                 /* 1 ≤ k ≤ r */
    idx_t diff = r - k;
    idx_t exp = diff * diff;                   /* (r-k)^2 ≤ r^2 ≤ 20000 for n≈10000 */

    return pow_seq_t((seq_t)k, exp);
}
