/*
 * A036701  a(n)=number of Gaussian integers z=a+bi satisfying n-1<|z|<=n,
 *          a>=0, 0<=b<a.
 * https://oeis.org/A036701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Gaussian integers a+bi with a>=0, 0<=b<a * and
 *            n-1 < |a+bi| <= n
 * Range:     u8 n <= 322, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root for unsigned 64‑bit values */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r   = 0;
    idx_t bit = (idx_t)1ULL << 62; /* highest power of four <= x */
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

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t count = 0;

    idx_t n2     = n * n;               /* n^2  */
    idx_t rem1   = (n - 1) * (n - 1);   /* (n-1)^2 */

    for (idx_t a = 1; a <= n; ++a) {
        idx_t a2 = a * a;
        idx_t max_b = isqrt_u64(n2 - a2);
        if (max_b >= a)
            max_b = a - 1; /* because b < a */

        idx_t min_b;
        if (a2 > rem1) {
            min_b = 0;
        } else {
            idx_t diff = rem1 - a2;
            min_b = isqrt_u64(diff) + 1;
        }

        if (min_b <= max_b)
            count += (seq_t)(max_b - min_b + 1);
    }

    return count;
}
