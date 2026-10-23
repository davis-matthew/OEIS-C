/*
 * A008491  Expansion of (1-x^9 ) / (1-x)^9.
 * https://oeis.org/A008491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n < 9) ? C(n+8,8) : C(n+8,8) - C(n-1,8)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 58, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C(n,k) is computed with a 128‑bit intermediate to avoid overflow */
/* for all supported seq_t (uint8_t … unsigned __int128, or arbitrary precision) */

#include <stdint.h>

static seq_t binom8(idx_t N)
{
    /* Compute C(N,8) for N >= 8.  The intermediate product is
       performed in an unsigned __int128 to guarantee no overflow for
       uint64_t seq_t (N up to at least 10 000).  For larger seq_t
       types the built‑in promotion is harmless. */
    seq_t res = 1;
    for (idx_t i = 1; i <= 8; ++i)
    {
        unsigned __int128 tmp = (unsigned __int128)res * (N - 8 + i);
        tmp /= i;
        res = (seq_t)tmp;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n < 9)
        return binom8(n + 8);
    return binom8(n + 8) - binom8(n - 1);
}
