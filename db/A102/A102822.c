/*
 * A102822  a(n+1) is the integer part of sqrt(2*a(n)^2).
 * https://oeis.org/A102822
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 65, u64 n <= 129,
 *            u128 n <= 256, big any n
 * Validated: standard, pass, 58 terms through n = 987 (2026-11-03)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102822: a(3) = 3, a(n+1) = floor(sqrt(2 a(n)^2)). At the fixed widths
 * 2 a(n)^2 is formed in 128 bits (it passes 2^64 while a(n) still fits; the
 * steps stop once a(n) > 2^63.5, where it would pass 2^128), for
 * big in seq_t. */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

#ifndef __cplusplus
typedef unsigned __int128 u128;

static u128 isqrt128(u128 v)
{
    u128 r = 0;
    for (u128 bit = (u128)1 << 126; bit; bit >>= 2) {
        if (v >= r + bit) { v -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
    }
    return r;
}
#endif

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;
#ifdef __cplusplus
    seq_t x = 3;
    for (idx_t i = 3; i < n; ++i) x = isqrt_seq((seq_t)2 * x * x);
    return x;
#else
    u128 x = 3;
    for (idx_t i = 3; i < n && x <= (u128)UINT64_C(0xB504F333F9DE6484); ++i) x = isqrt128(2 * x * x);
    return (seq_t)x;
#endif
}
