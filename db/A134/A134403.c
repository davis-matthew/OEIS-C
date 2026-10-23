/*
 * A134403  Triangle read by rows: row n consists of (n, n, (n+1), (n+2),
 *          (n+3), ...).
 * https://oeis.org/A134403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r if p <= 1 else r + p - 1, where r = floor((sqrt(8*n +
 *            1) - 1) / 2) and p = n - r*(r + 1)/2
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

/* Integer square root of a seq_t value using binary search. */
static inline seq_t isqrt_seq(seq_t x)
{
    if (x <= 1) return x;            /* 0 or 1 */
    seq_t low = 1;
    seq_t high = x;
    seq_t ans = 1;
    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        seq_t div = x / mid;
        if (mid <= div) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Compute r = floor((sqrt(8*n + 1) - 1) / 2) */
    seq_t t = (seq_t)n * 8 + 1;            /* 8*n + 1 */
    seq_t sqrt_t = isqrt_seq(t);           /* floor(sqrt(t)) */
    seq_t r = (sqrt_t - 1) / 2;            /* r may be up to ~2^32 */

    /* Compute s = r*(r + 1)/2 safely to avoid overflow */
    seq_t s;
    if (r & 1) {
        s = r * ((r + 1) / 2);
    } else {
        s = (r / 2) * (r + 1);
    }

    /* Position within the row */
    seq_t p = (seq_t)n - s;                /* p >= 0 */

    /* Value according to the triangle rule */
    if (p <= 1) {
        return r;
    } else {
        return r + p - 1;
    }
}
