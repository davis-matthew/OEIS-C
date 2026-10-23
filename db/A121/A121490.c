/*
 * A121490  Rectangular array T by antidiagonals: T(n,k) = [y+1]^2-y^2, where
 *          y=n*sqrt(k) and [ ] denotes the floor function.
 * https://oeis.org/A121490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(y+1)^2 – y^2 with y = i * sqrt(j)\n where (i,j) are
 *            the unique coordinates obtained by\n traversing the infinite
 *            array T(n,k) by antidiagonals
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt_uint64(uint64_t x)
{
    /* Integer square root (floor of sqrt) using bitwise algorithm. */
    uint64_t r = 0, bit = 1ULL << 62;  /* largest power of 4 <= x */
    while (bit > x) bit >>= 2;
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

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset 1 – index 0 unused */

    /* Find diagonal d such that d*(d+1)/2 >= n. */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;

    idx_t prev = (d - 1) * d / 2;
    idx_t pos  = n - prev;          /* 1 … d */

    idx_t i = pos;                 /* row index   (1‑based) */
    idx_t j = d - pos + 1;          /* column index (1‑based) */

    /* Compute y^2 = i^2 * j in 64‑bit, then floor sqrt. */
    uint64_t prod = (uint64_t)i * (uint64_t)i * (uint64_t)j;
    uint64_t f    = isqrt_uint64(prod);
    uint64_t t    = f + 1ULL;

    /* a(n) = t^2 – prod.  The intermediate values fit in 64‑bit. */
    return (seq_t)(t * t - prod);
}
