/*
 * A135481  a(n) = 2^A007814(n+1) - 1.
 * https://oeis.org/A135481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^A007814(n+1) - 1
 * Range:     u8 n <= 510, u16 n <= 9699, u32 n <= 9699, u64 n <= 9699,
 *            u128 n <= 9699, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(n) = 0 for even n because A007814(n+1) = 0 there */
    if ( (n & 1ULL) == 0 )
        return 0;

    /* v = (n+1)/2  , computed without overflow */
    idx_t v = (n >> 1) + 1;

    /* count trailing zeros of v */
    unsigned int tz = 0;
    while ( (v & 1ULL) == 0 )
    {
        tz++;
        v >>= 1;
    }

    unsigned int e = tz + 1;               /* exponent for 2^e */
    /* handle the only problematic case: 2^64 for 64‑bit seq_t */
    if ( e == 64 && sizeof(seq_t) == 8 )
        return (seq_t)-1;                  /* 2^64 - 1 in 64‑bit unsigned */

    seq_t val = big_pow( (seq_t)2, (seq_t)e ); /* compute 2^e exactly */
    return val - (seq_t)1;
}
