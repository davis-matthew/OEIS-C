/*
 * A129234  Triangle read by rows: T(n,k) = n/k + k - 1 if n mod k = 0;
 *          otherwise T(n,k)=0 (1 <= k <= n).
 * https://oeis.org/A129234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle * T(n,k) = n/k + k - 1
 *            if n mod k == 0 * T(n,k) = 0 otherwise * with 1 <= k <= n, row
 *            by row, left to right. a(1) = 1, a(2) = 2,
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer:
 * returns ⌊√x⌋ using only integer operations. */
static idx_t floor_isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;          /* largest power of four <= 2^64-1 */

    /* Adjust bit to be <= x */
    while (bit > x)            bit >>= 2;

    while (bit) {
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

/* OEIS A129234 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe guard */

    /* Find the row r such that triangular(r-1) < n <= triangular(r) */
    idx_t disc   = 8 * n + 1;                     /* 8n+1 is always a perfect square
                                                     for the upper bound of row */
    idx_t root   = floor_isqrt_u64(disc);         /* ⌊√(8n+1)⌋ */
    idx_t r      = (root - 1) / 2;                /* initial estimate of row number */
    idx_t tri    = r * (r + 1) / 2;               /* triangular number of row r */

    if (tri < n) {                               /* adjust if needed */
        ++r;
        tri = r * (r + 1) / 2;
    }

    /* Position of n within the row (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k   = n - prev_tri;                    /* 1 ≤ k ≤ r */

    /* Compute T(r,k) */
    if ((r % k) == 0) {
        seq_t q   = (seq_t)(r / k);
        seq_t val = q + (seq_t)k - 1;
        return val;
    } else {
        return 0;
    }
}
