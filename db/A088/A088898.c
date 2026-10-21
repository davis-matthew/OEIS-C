/*
 * A088898  T(n,k) = number of ordered pairs of integers (x,y) with x^2/n^2 +
 *          y^2/k^2 < 1, 1<=k<=n; triangular array, read by rows.
 * https://oeis.org/A088898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the row‑by‑row reading of the triangle T(r,k) where
   T(r,k) = number of integer lattice points (x,y) satisfying
            x^2 / r^2 + y^2 / k^2 < 1,   1 ≤ k ≤ r.
   The mapping is n = r(r-1)/2 + k  (1‑based).  This function
   computes a(n) exactly using only integer arithmetic. */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for any seq_t. */
static inline seq_t isqrt(seq_t x)
{
    seq_t low = 0, high = x, ans = 0;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t sq  = mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Count ordered pairs (x,y) with x^2/r^2 + y^2/k^2 < 1, 1 ≤ k ≤ r. */
static inline seq_t triangle_elem(idx_t r, idx_t k)
{
    seq_t n   = (seq_t)r;
    seq_t nk  = (seq_t)k;
    seq_t n2  = n * n;
    seq_t k2  = nk * nk;
    seq_t total = 0;

    for (idx_t xi = 0; xi < r; ++xi) {
        seq_t x = (seq_t)xi;
        seq_t rem = n2 - x * x;            /* n² – x²  > 0 */
        seq_t num = k2 * rem;              /* k²·(n²–x²) */
        seq_t t   = (num - 1) / n2;        /* floor((k²·rem – 1)/n²) */
        seq_t ymax = isqrt(t);
        seq_t count = 2 * ymax + 1;        /* y = 0, ±1,…,±ymax */

        if (xi == 0)            /* x = 0 counted once */
            total += count;
        else                    /* x and –x distinct */
            total += 2 * count;
    }
    return total;
}

/* Main entry: a(n) for sequence A088898. */
seq_t a(idx_t n)
{
    /* Find row r such that r(r-1)/2 < n ≤ r(r+1)/2. */
    idx_t r   = 1;
    idx_t tri = 1;               /* r(r+1)/2 for current r */
    while (tri < n) {
        ++r;
        tri += r;
    }
    idx_t prev = tri - r;        /* (r-1)r/2 */
    idx_t k = n - prev;          /* column index (1‑based) */

    return triangle_elem(r, k);
}
