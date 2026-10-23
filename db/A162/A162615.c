/*
 * A162615  Triangle read by rows in which row n lists n terms, starting with
 *          n, such that the difference between successive terms is equal to
 *          n^3 - 1 = A068601(n).
 * https://oeis.org/A162615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 150, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A162615: Triangle with row n having n terms, first term = n,
   successive difference = n^3-1.
   Formula: a(n) = r + (k-1)*(r^3-1) where r is the row number such that
   triangular(r-1) < n ≤ triangular(r) and k = n - triangular(r-1). */
#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    /* floor of sqrt(n) for unsigned 64‑bit integer */
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) { ans = 0; lo = 1; continue; }
        if (mid <= n / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Find row r using: r = ceil((sqrt(8*n+1)-1)/2) */
    idx_t t = 8 * n + 1;
    idx_t s = isqrt_u64(t);
    idx_t r = (s - 1) >> 1;
    if (( (seq_t)r * ((seq_t)r + 1) ) / 2 < (seq_t)n)
        r++;

    /* Column k = n - triangular(r-1) */
    idx_t prev_row_sum = (r > 1) ? ((r - 1) * r / 2) : 0;
    idx_t k = n - prev_row_sum;          /* 1 ≤ k ≤ r */

    /* Compute a(n) = r + (k-1)*(r^3-1) */
    seq_t rr = (seq_t)r;
    seq_t diff = rr * rr * rr - 1;        /* r^3 - 1 */
    seq_t result = rr + ( (seq_t)(k - 1) ) * diff;
    return result;
}
