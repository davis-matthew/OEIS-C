/*
 * A036695  a(n)=number of Gaussian integers z=a+bi satisfying |z|<=n, b>=0.
 * https://oeis.org/A036695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{y=0}^{n} (2*⌊√(n² – y²)⌋ + 1)
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* count integer pairs (x, y) with x² + y² <= n² and y >= 0 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;

    for (idx_t y = 0; y <= n; ++y)
    {
        seq_t y2   = (seq_t)y * (seq_t)y;
        seq_t val  = N2 - y2;          /* 0 <= val <= N2          */
        seq_t root = floor_sqrt(val);  /* ⌊√val⌋                  */
        sum += 2 * root + 1;
    }
    return sum;
}
