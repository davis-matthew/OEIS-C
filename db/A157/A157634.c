/*
 * A157634  Triangle T(n, k) = 1 if k = 0 or k = n, otherwise n^5 - k^5 -
 *          (n-k)^5, read by rows.
 * https://oeis.org/A157634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if k=0 or k=n, * otherwise a(n) = 5*n*k*(n-k)*(n^2 -
 *            n*k + k^2), * where n is the triangular row number, * k = n -
 *            r(r+1)/2, * r = floor((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 10, u16 n <= 56, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for 64‑bit values */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;               /* highest power of two that fits in 64 bits */

    /* bring bit down to the largest square <= x */
    while (bit > x) bit >>= 2;

    /* binary search */
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t m = (n << 3) + 1;                /* 8*n + 1 */
    idx_t s = isqrt_u64(m);                /* floor(sqrt(8*n+1)) */
    idx_t r = (s - 1) >> 1;                /* floor((sqrt-1)/2) */

    idx_t off = r * (r + 1) / 2;            /* offset of row r */
    idx_t k = n - off;                     /* column index within the row */

    /* vertices of the triangle */
    if (k == 0 || k == r) {
        return (seq_t)1;
    }

    /* compute the expression in integer arithmetic only */
    seq_t ns = (seq_t)r;
    seq_t ks = (seq_t)k;
    seq_t nk = ns - ks;                    /* n-k */

    seq_t prod = ns * ks;                  /* n*k */
    prod = prod * nk;                      /* n*k*(n-k) */

    seq_t np2 = ns * ns;                   /* n^2 */
    seq_t kp2 = ks * ks;                   /* k^2 */
    seq_t expr = np2 + kp2;                /* n^2 + k^2 */
    expr = expr - ns * ks;                 /* n^2 + k^2 - n*k */

    seq_t val = 5;
    val = val * prod;                      /* 5*n*k*(n-k) */
    val = val * expr;                      /* ...*(n^2 - n*k + k^2) */

    return val;
}
