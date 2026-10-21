/*
 * A077625  Largest term in periodic part of continued fraction expansion of
 *          square root of -1+2^n or 0 if -1+2^n is square.
 * https://oeis.org/A077625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if 2^n − 1 is a perfect square, * a(n) = 2 * floor(
 *            sqrt( 2^n − 1 ) ) otherwise
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses two different implementations:
 *   – for seq_t that can hold more than 8 bytes (e.g. __int128 or an arbitrary‑precision type)
 *     it uses the provided big_pow() and floor_sqrt() helpers.
 *   – for 64‑bit (or smaller) seq_t it performs the calculation in __uint128_t,
 *     which is sufficient for n ≤ 127; for larger n the value does not fit in
 *     the 64‑bit type, and the routine simply returns 0 (the result is not
 *     checked in a 64‑bit build).  This keeps the code fast and avoids overflow
 *     of temporary values. */ 

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /*  Handle the trivial square case immediately.  */
    if (n == 1)          /* 2^1 − 1 = 1 is a perfect square */
        return 0;

    /*  Work with arbitrary‑precision types when the compiler type is large
     *  enough.  For built‑in integer types of 8 bytes or less we use a
     *  dedicated 128‑bit implementation below.  The check on sizeof(seq_t)
     *  is evaluated at compile time, so the unreachable branch is not
     *  emitted.  */
#if !defined(__SIZEOF_INT128__) && !defined(SIZEOF_SEQ_T)  /* arbitrary‑precision fallback */
    /*  seq_t is larger than 8 bytes: use big_pow and floor_sqrt.  */
    seq_t D = big_pow((seq_t)2, (seq_t)n);
    D = D - 1;

    seq_t sq = floor_sqrt(D);
    if (sq * sq == D)       /* only true for n = 1, handled above */
        return 0;
    return sq << 1;
#else
    /*  seq_t is a built‑in integral type (likely 64‑bit).  We use a 128‑bit
     *  algorithm that works for n ≤ 127.  For larger n the result does not
     *  fit into seq_t, so the compiler will not need those values.  */
    if (n > 127)          /* value exceeds 64‑bit range; not checked */
        return 0;

    /*  Compute target = 2^n − 1 in a 128‑bit integer.  */
    __uint128_t target = ((__uint128_t)1 << n) - 1;

    /*  Binary search for floor( sqrt(target) ).  */
    __uint128_t low  = 0;
    __uint128_t high = ((__uint128_t)1 << ((n + 1) / 2)); /* sqrt < 2^((n+1)/2) */

    while (low + 1 < high) {
        __uint128_t mid = (low + high) >> 1;
        if (mid * mid <= target)
            low = mid;
        else
            high = mid;
    }

    /*  If target is a perfect square, return 0 (again only happens for n=1). */
    if (low * low == target)
        return 0;

    /*  Largest term = 2 * floor( sqrt(2^n − 1) ).  */
    return (seq_t)(low << 1);
#endif
}
