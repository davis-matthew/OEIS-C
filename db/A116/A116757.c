/*
 * A116757  Number of permutations of length n which avoid the patterns 1324,
 *          2314, 4312.
 * https://oeis.org/A116757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4) * ( -4 + 2^(3+n) - (2 + 5*2^n)*n + (2 + 2^n)*n^2 )
 *            (the index starts at 1, a(1)=1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 54, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* ------- C implementation (uses 128‑bit integers for safety) ------- */
#ifndef __cplusplus
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* value for indices < 1 is irrelevant */

    /* compute 2^n in a 128‑bit signed integer */
    __int128 two_pow_n = 1;
#if defined(_MSC_VER)
    for (idx_t i = 0; i < n; ++i) two_pow_n <<= 1;
#else
    for (idx_t i = 0; i < n; ++i) two_pow_n <<= 1;
#endif

    /* intermediate terms, all in signed 128‑bit */
    __int128 term1 = (__int128)8 * two_pow_n;                                 /* 2^(n+3) */
    __int128 term2 = (__int128)(2 + (__int128)5 * two_pow_n) * n;             /* (2+5*2^n)*n */
    __int128 term3 = (__int128)(2 + two_pow_n) * n * n;                      /* (2+2^n)*n^2 */

    __int128 numerator = term1 + term3 - term2 - (__int128)4;                /* (-4 + ...) */
    __int128 result128 = numerator >> 2; /* divide by 4 */

    return (seq_t)result128;             /* cast back to seq_t (uint64_t in this build) */
}
#else
/* ------- C++ implementation (uses seq_t operations directly) ------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* value for indices < 1 is irrelevant */

    /* compute 2^n as seq_t (pow2 is provided in utils) */
    seq_t two_pow_n = pow2(n);           /* 2^n */

    /* intermediate terms as seq_t */
    seq_t term1 = two_pow_n * (seq_t)8u;                              /* 2^(n+3) */
    seq_t term2 = (seq_t)( (seq_t)2u + (seq_t)5u * two_pow_n ) * (seq_t)n;   /* (2+5*2^n)*n */
    seq_t term3 = (seq_t)( (seq_t)2u + two_pow_n ) * (seq_t)n * (seq_t)n;   /* (2+2^n)*n^2 */

    seq_t numerator = term1 + term3 - term2 - (seq_t)4u;
    return numerator / (seq_t)4u;
}
#endif
