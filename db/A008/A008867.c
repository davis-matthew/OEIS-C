/*
 * A008867  Triangle of truncated triangular numbers: k-th term in n-th row is
 *          number of dots in hexagon of sides k, n-k, k, n-k, k, n-k.
 * https://oeis.org/A008867
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(r,k) = (r+1)(r+2)/2 + r*k – k^2 where r is the row index
 *            (starting at 0) and k is the column index within that row (0 ≤ k
 *            ≤ r). The sequence a(n) is read by rows starting with a(2) as
 *            the first element of row 0, a(3) as the first element of row 1,
 *            and so on
 * Range:     u8 n <= 178, u16 n <= 1226, u32 n <= 1226, u64 n <= 1226,
 *            u128 n <= 1226, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;                 /* offset is 2, below it is undefined */

    /* zero‑based position within the triangle */
    idx_t idx = n - 2;

    /* compute row index r:  r = floor((sqrt(1+8*idx)-1)/2)   */
    long double disc = 1.0L + 8.0L * (long double)idx;
    long double root = sqrtl(disc);
    unsigned long long r = (unsigned long long)((root - 1.0L) * 0.5L);

    /* fix possible rounding errors */
    while ( (r * (r + 1) / 2) > idx ) r--;
    while ( ((r + 1) * (r + 2) / 2) <= idx ) r++;

    /* column index k */
    idx_t prev = r * (r + 1) / 2;
    idx_t k     = idx - prev;

    /* compute the term using the triangular‑number formula */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t first = (rr + 1) * (rr + 2) / 2;
    seq_t val   = first + kk * rr - kk * kk;

    return val;
}
