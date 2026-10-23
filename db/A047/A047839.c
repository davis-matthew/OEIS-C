/*
 * A047839  a(n) = n + floor( sqrt(2*n) ).
 * https://oeis.org/A047839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor( sqrt(2*n) )
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of x (floor sqrt) for 64‑bit unsigned integers.
   Uses binary search with 128‑bit intermediate products to avoid overflow. */
static idx_t _isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t b = ((idx_t)1ULL << 32);   /* initial step size */
    while (b) {
        idx_t t = r + b;
        /* use 128‑bit multiplication to avoid overflow */
        if ((unsigned __int128)t * t <= (unsigned __int128)x)
            r = t;
        b >>= 1;
    }
    return r;
}

/* OEIS sequence A047839 */
seq_t a(idx_t n)
{
    /* compute n + floor(sqrt(2*n)) */
    seq_t nn = (seq_t)n;
    idx_t two_n = n << 1;          /* 2*n, safe for n up to ~1e19 */
    seq_t sqrtval = (seq_t)_isqrt_u64(two_n);
    return nn + sqrtval;
}
