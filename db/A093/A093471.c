/*
 * A093471  a(n) = floor(n^(1/2)*10^n).
 * https://oeis.org/A093471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(n)*10^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   floor_sqrt, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"      /* seq_t pow10(idx_t e); */
#include "../utils/floor_sqrt.h" /* seq_t floor_sqrt(seq_t x); */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;            /* Undefined for offset 0, return 0 for safety */

    /* For seq_t that fits in 64 bits (typical uint64_t) use an explicit
     * 128‑bit implementation to avoid overflow of pow10(2*n). */
    if (sizeof(seq_t) <= 8u) {
        /* The value fits in 64 bits only up to n = 18 (a(18) < 2^63). */
        if (n > 18u)
            return 0;        /* Out of range for 64‑bit seq_t; caller checks only
                                 terms that fit in seq_t. */

        /* Compute 10^(2*n) in 128 bits. */
        idx_t exp = 2 * n;
        __int128 p = 1;
        for (idx_t i = 0; i < exp; ++i)
            p *= 10;

        /* Compute big = n * 10^(2*n)  (fits in 128 bits for n <= 18). */
        __int128 big = (__int128)n * p;

        /* Integer square root of the 128‑bit number. */
        __int128 low = 0;
        __int128 high = 1;
        while (high * high <= big)
            high <<= 1;               /* high *= 2 */

        while (low + 1 < high) {
            __int128 mid = (low + high) >> 1;
            if (mid * mid <= big)
                low = mid;
            else
                high = mid;
        }
        return (seq_t)low;
    }

    /* For arbitrary–precision seq_t use the library functions. */
    idx_t exp = 2 * n;
    seq_t p10 = pow10(exp);
    seq_t x   = seq_mul(p10, (seq_t)n);
    return floor_sqrt(x);
}
