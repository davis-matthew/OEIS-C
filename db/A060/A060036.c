/*
 * A060036  Triangular array T read by rows: T(n,k) = k^2 mod n, for k =
 *          1,2,...,n-1, n = 2,3,...
 * https://oeis.org/A060036
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = (k^2) mod r, where r and k are determined by *
 *            linearising the triangular array T(r,k)=k^2 mod r for r≥2, 1≤k<r
 * Range:     u8 n <= 4951, u16 n <= 4951, u32 n <= 4951, u64 n <= 4951,
 *            u128 n <= 4951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt_uint64(uint64_t v)
{
    /* integer square root via binary search */
    uint64_t lo = 0, hi = v;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {
            lo = mid + 1;
            continue;
        }
        if (mid > v / mid) {          /* mid*mid would overflow or > v */
            hi = mid - 1;
        } else {
            uint64_t sq = mid * mid;
            if (sq == v) return mid;
            if (sq < v) lo = mid + 1;
            else hi = mid - 1;
        }
    }
    return hi;   /* floor(sqrt(v)) */
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    uint64_t idx = n - 2;                 /* zero‐based position inside the triangle */

    /* Find row r such that (r-1)*r/2 > idx. */
    uint64_t d = 1 + 8 * idx;             /* discriminant for quadratic formula */
    uint64_t s = isqrt_uint64(d);         /* floor(sqrt(d)) */
    uint64_t r = (s + 1) / 2 + 1;         /* row index */

    /* Position within the row (0‑based) */
    uint64_t prevSum = (r - 2) * (r - 1) / 2;
    uint64_t pos = idx - prevSum;
    uint64_t k = pos + 1;                 /* column index (1‑based) */

    seq_t rn = (seq_t)r;
    seq_t km = (seq_t)k;
    return (km * km) % rn;
}
