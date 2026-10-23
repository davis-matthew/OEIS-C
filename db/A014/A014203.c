/*
 * A014203  a(n) = Sum_{i,j,k in Z and i^2 + j^2 + k^2 <= n} i^2 + j^2 + k^2.
 * https://oeis.org/A014203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i,j,k in Z : i^2 + j^2 + k^2 <= n} (i^2 + j^2 + k^2)
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple integer square‑root routine and double
   nested loops to enumerate all lattice points inside the sphere of squared
   radius n. It runs in O(n^(3/2)) time, which is fast enough for n up to
   about 10 000. */

#include <stddef.h>

/* Integer square root for uint64_t values, defined in a helper function to
   avoid the missing external dependency _floor_isqrt_u64. */
static inline idx_t _isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + (n / x)) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    /* radius of sphere in integer coordinates */
    idx_t R = _isqrt_u64(n);

    for (int i = -(int)R; i <= (int)R; ++i) {
        /* compute i^2 safely as a signed product */
        idx_t i2 = (idx_t)((long long)i * (long long)i);
        if (i2 > n)
            continue;                  /* skip if i^2 alone exceeds n */
        idx_t rem_i = n - i2;
        idx_t jR = _isqrt_u64(rem_i);

        for (int j = -(int)jR; j <= (int)jR; ++j) {
            idx_t j2 = (idx_t)((long long)j * (long long)j);
            if (i2 + j2 > n)            /* prune early */
                continue;
            idx_t rem_j = rem_i - j2;
            idx_t kR = _isqrt_u64(rem_j);

            for (int k = -(int)kR; k <= (int)kR; ++k) {
                idx_t k2 = (idx_t)((long long)k * (long long)k);
                seq_t d = (seq_t)(i2 + j2 + k2);
                total += d;
            }
        }
    }
    return total;
}
