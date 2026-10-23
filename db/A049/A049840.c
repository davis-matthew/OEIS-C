/*
 * A049840  Triangular array T read by rows: T(n,k) = sum of numbers c in the
 *          form c = bq + r when Euclidean algorithm acts on n, k, for 1 <= k
 *          <= n, n >= 1.
 * https://oeis.org/A049840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) entry of the triangular array T read by rows,
 *            where T(n,k) = n + T(k, n mod k) and T(n,0)=0. T is computed by
 *            the ordinary Euclidean algorithm, summing the "first" arguments
 *            until the second argument becomes zero
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required types provided by the build environment:
   seq_t  : unsigned integer type (8,16,32,64-bit or arbitrary‑precision)
   idx_t  : unsigned 64‑bit integer (uint64_t) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* not part of the OEIS offset */

    /* Find row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n.
       Binary search using 128‑bit arithmetic to avoid overflow. */
    idx_t lo = 1, hi = n, r = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n) {
            r = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    /* Number of terms before this row */
    idx_t prev = (idx_t)(((__int128)(r - 1) * r) / 2);
    /* Column within the row (1‑based) */
    idx_t k = n - prev;

    /* Compute T(r,k) by iterative Euclidean algorithm,
       summing the first argument at each step until k becomes 0. */
    seq_t sum = 0;
    seq_t x = (seq_t)r;
    seq_t y = (seq_t)k;
    while (y != 0) {
        sum += x;
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return sum;
}
