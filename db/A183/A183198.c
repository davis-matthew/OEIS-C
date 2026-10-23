/*
 * A183198  Least integer k such that Floor(k*(n+1)^(2/3))<k*n^(2/3).
 * https://oeis.org/A183198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer k >= 1 such that floor(k * n^(2/3)) <
 *            floor(k * (n+1)^(2/3))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer cube root for a non‑negative 128‑bit integer.
   Returns floor(cbrt(x)). */
static inline seq_t cube_root_floor(__int128 x)
{
    if (x <= 0) return (seq_t)0;

    /* Find a high bound: power of two > cbrt(x). */
    uint64_t low = 0;
    uint64_t high = 1;
    while ((__int128)high * high * high <= x) high <<= 1;

    /* Binary search on [low,high]. */
    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;
        __int128 mid3 = (__int128)mid * mid * mid;
        if (mid3 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}

seq_t a(idx_t n)
{
    /* OEIS defines a(1) as the first term, the formula makes sense
       only for n >= 1. The function may be called with n == 0,
       but in that case we treat it as if n == 1. */
    if (n == 0) n = 1;

    for (idx_t k = 1; ; ++k) {
        /* f_n(k) = floor( (k^3 * n^2)^(1/3) ) */
        __int128 prod_n  = (__int128)k * k * k * n * n;
        __int128 prod_np1 = (__int128)k * k * k * (n + 1) * (n + 1);

        seq_t fn   = cube_root_floor(prod_n);
        seq_t fnp1 = cube_root_floor(prod_np1);

        if (fn < fnp1) return (seq_t)k;
    }
}
