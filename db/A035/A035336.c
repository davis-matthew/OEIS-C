/*
 * A035336  a(n) = 2*floor(n*phi) + n - 1, where phi = (1+sqrt(5))/2.
 * https://oeis.org/A035336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor(n*phi) + n - 1, phi = (1+sqrt(5))/2
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute floor(n * sqrt(5)) by integer binary search.
   Works for any n that does not make 5*n*n overflow a 128‑bit
   signed integer (sufficient for n up to at least 1e18). */
static inline seq_t floor_n_sqrt5(idx_t n)
{
    __int128 idx = ( __int128 )n;
    __int128 low  = 2 * idx;            /* sqrt(5) > 2  */
    __int128 high = 3 * idx;            /* sqrt(5) < 3  */
    __int128 target = 5 * idx * idx;    /* 5*n^2 */

    while (low < high)
    {
        __int128 mid  = (low + high + 1) >> 1;
        __int128 mid2 = mid * mid;
        if (mid2 <= target)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;  /* floor(n*sqrt(5)) */
}

seq_t a(idx_t n)
{
    /* floor(n*phi) = floor( (n + floor(n*sqrt(5)) ) / 2 ) */
    seq_t fn_sqrt5 = floor_n_sqrt5(n);
    seq_t floor_n_phi = ((seq_t)n + fn_sqrt5) / 2;

    /* a(n) = 2*floor(n*phi) + n - 1 */
    return 2 * floor_n_phi + (seq_t)n - 1ULL;
}
