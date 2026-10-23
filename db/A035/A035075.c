/*
 * A035075  a(n) = ceiling(sqrt(8*10^n)).
 * https://oeis.org/A035075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035075: a(n) = ceil(sqrt(8*10^n)) */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

/* Compute 10^e in unsigned __int128 for small e (e <= 37). */
static unsigned __int128 pow10_uint128(idx_t e)
{
    unsigned __int128 res = 1;
    for (idx_t i = 0; i < e; ++i)
        res *= 10;
    return res;
}

/* Integer square root for unsigned __int128: floor(sqrt(x)). */
static unsigned __int128 sqrt_uint128(unsigned __int128 x)
{
    if (x == 0 || x == 1)
        return x;
    unsigned __int128 low = 0;
    unsigned __int128 high = (x >> 1) + 1;
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute a(n) for any n >= 1.  For n <= 37 we use 128‑bit math
   so that intermediate values never overflow a uint64_t.  For
   larger n the arbitrary‑precision helpers are employed. */
seq_t a(idx_t n)
{
    /* n is unsigned, but we only need to check it against 37. */
    if (n <= 37) {
        unsigned __int128 t = pow10_uint128(n);
        t *= 8;
        unsigned __int128 s = sqrt_uint128(t);
        if (s * s < t)
            ++s;
        return (seq_t)s;
    }

    /* General case: use power function and floor_sqrt from the utility set. */
    seq_t pow10n = pow10(n);       /* 10^n as seq_t */
    seq_t target = pow10n * 8;     /* 8 * 10^n */
    seq_t s = floor_sqrt(target);  /* floor sqrt(target) */
    if (s * s < target)
        ++s;                       /* ceil sqrt */
    return s;
}
