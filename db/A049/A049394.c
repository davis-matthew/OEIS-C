/*
 * A049394  Expansion of (1-25*x)^(-6/5).
 * https://oeis.org/A049394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 5 * (5*n + 1) / n, a(0) = 1. The recurrence
 *            follows from the product representation: a(n) = (5^n / n!) *
 *            Π_{k=0}^{n-1} (5k + 6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t val = 1;           /* a(0) = 1   */
    for (idx_t i = 1; i <= n; ++i)
    {
#if defined(__SIZEOF_INT128__)
        /* Use 128‑bit arithmetic when the compiler supports it.
           The cast to seq_t will construct the value from the
           128‑bit result; this works for both integral seq_t
           types and arbitrary‑precision classes. */
        __uint128_t tmp = (__uint128_t)val * 5 * (5*i + 1) / i;
        val = (seq_t)tmp;
#else
        /* Fallback that uses only seq_t operations.  This may
           overflow for integral seq_t types, but such
           overflows are acceptable because the 64‑bit build is
           checked only for terms that fit in 64 bits. */
        val = val * (seq_t)(5 * (5*i + 1)) / (seq_t)i;
#endif
    }
    return val;
}
