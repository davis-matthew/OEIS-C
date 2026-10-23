/*
 * A162611  Triangle read by rows in which row n lists n+1 terms, starting
 *          with n, such that the difference between successive terms is equal
 *          to n^2 - 1.
 * https://oeis.org/A162611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = row(n) + k*(row(n)^2-1), where * row(n) = floor((-1 +
 *            sqrt(1+8n))/2) and * k = n - row(row(n)+1)/2. * This yields the
 *            triangle defined in A162611
 * Range:     u8 n <= 33, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer sqrt for unsigned 64‑bit values, using 128‑bit arithmetic
 * to avoid overflow.  Returns floor(sqrt(x)). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;       /* mid = ceil((lo+hi)/2) */
        if ((__int128)mid * mid <= x) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

/* Sequence A162611 */
seq_t a(idx_t n)
{
    /* 1 + 8*n fits in idx_t for all valid n (k<2^64) */
    idx_t d = 1 + (idx_t)8 * n;

    /* r = floor((-1 + sqrt(d))/2) */
    idx_t s = isqrt_u64(d);
    idx_t r = (s - (idx_t)1) >> 1;            /* same as (s-1)/2 */

    /* t_r = r*(r+1)/2, the total number of terms up to row r-1 */
    idx_t t_r = (r * (r + (idx_t)1)) >> 1;    /* divide by 2 */

    /* position k within the current row */
    idx_t k = n - t_r;

    /* compute the term: r + k*(r^2-1) */
    seq_t diff = (seq_t)r * (seq_t)r - (seq_t)1;
    seq_t term = (seq_t)r + (seq_t)k * diff;
    return term;
}
