/*
 * A083048  Main diagonal of table A083047.
 * https://oeis.org/A083048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n, n) where * T(n,0) = floor(n*x/(x-1)) + 1 * T(n,k+1)
 *            = ceil(x*T(n,k)) * with x = (sqrt(5)+1)/2 (golden ratio). * *
 *            Implementation notes: * • Since x is irrational, ceil(x*y) =
 *            floor(x*y) + 1 for any integer y > 0. * • floor(y*phi) is
 *            computed as (y + floor_sqrt(5*y*y)) / 2. * • Use a 1...
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* Integer square root for 128‑bit values.
 * Returns ⌊sqrt(x)⌋. WHOLE FUNCTION IS INTERNAL. */
static inline uint64_t isqrt128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126;  /* largest power of four <= x */

    while (bit > x)
        bit >>= 2;

    while (bit != 0)
    {
        if (x >= r + bit)
        {
            x -= r + bit;
            r = (r >> 1) + bit;
        }
        else
        {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (uint64_t)r;
}

/* Compute floor(phi * v) where phi = (1+sqrt(5))/2.
 * For 64‑bit seq_t values a 128‑bit intermediate is used.
 * For arbitrary‑precision seq_t, floor_sqrt is used. */
static inline seq_t floor_phi(seq_t v)
{
#if defined(__SIZEOF_INT128__)
    /* 64‑bit build: use 128‑bit intermediate that fits for all values
       that still fit in 64‑bit. */
    unsigned __int128 vv = (unsigned __int128)v;
    unsigned __int128 prod = 5u * vv * vv;           /* 5 * v * v   */
    uint64_t sqrt128 = isqrt128(prod);              /* floor(sqrt) */
    return (v + (seq_t)sqrt128) / 2;
#else
    /* Arbitrary‑precision build: rely on the provided floor_sqrt
       which supports big integers.                 */
    seq_t sqrtv = floor_sqrt((seq_t)5 * v * v);
    return (v + sqrtv) / 2;
#endif
}

/* Main function to compute a(n) for OEIS A083048. */
seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* Compute T(n,0) = floor(n*phi) + n + 1. */
    seq_t floor_n_phi = floor_phi(nseq);
    seq_t cur = nseq + floor_n_phi + 1;   /* T(n,0) */

    /* Advance k from 0 to n-1 to reach T(n,n). */
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t floor_cur_phi = floor_phi(cur);
        cur = floor_cur_phi + 1;          /* ceil(phi*cur) */
    }

    return cur;
}
