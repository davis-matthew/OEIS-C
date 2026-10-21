/*
 * A083920  Number of nontriangular numbers <= n.
 * https://oeis.org/A083920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - floor((sqrt(8*n+1)-1)/2) – number of non‑triangular
 *            numbers <= n
 * Range:     u8 n <= 278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

/* Integer square root of a 64‑bit unsigned integer.
   Returns floor(sqrt(x)). Uses only integer arithmetic. */
static inline idx_t isqrt_uint64(idx_t x)
{
    idx_t r = 0;
    /* Start with the largest power of 4 that fits in 64 bits: 1 << 62 */
    idx_t bit = ((idx_t)1) << 62;
    while (bit > x)
        bit >>= 2;

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
    /* Compute k = floor((sqrt(8*n+1)-1)/2), the largest t such that t*(t+1)/2 <= n */
    idx_t val  = 8 * n + 1;          /* 8*n + 1 does not overflow idx_t for n <= 2^62-1 */
    idx_t tsqrt = isqrt_uint64(val); /* floor(sqrt(8*n+1)) */
    idx_t k     = (tsqrt - 1) >> 1;   /* floor((sqrt(8*n+1)-1)/2) */

    /* a(n) = n - k */
    return (seq_t)(n - k);
}
