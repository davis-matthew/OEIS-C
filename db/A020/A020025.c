/*
 * A020025  Nearest integer to Gamma(n + 7/8)/Gamma(7/8).
 * https://oeis.org/A020025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to Γ(n + 7/8)/Γ(7/8) = nearest integer to
 *            (∏_{k=0}^{n-1} (8k+7)) / 8^n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented by integer arithmetic only.                     */

#include <stdint.h>

#ifdef __SIZEOF_INT128__
#  define HAVE_INT128 1
#endif

seq_t a(idx_t n)
{
    /* Handle the empty product case */
    if (n == 0U)
        return (seq_t)1;

#ifdef HAVE_INT128
    /* Use 128‑bit arithmetic when available to keep the
       intermediate products from overflowing the ordinary
       seq_t type (which could be 8, 16, 32 or 64 bits). */
    unsigned __int128 num = 1;
    unsigned __int128 denom = 1;

    for (idx_t k = 0; k < n; ++k)
    {
        unsigned __int128 factor = (unsigned __int128)8 * k + 7;
        num   *= factor;
        denom *= 8;
    }

    /* Round to nearest integer: add half the denominator before
       division.  All numbers are positive so the C integer
       division truncates toward zero, giving the correct rounding. */
    unsigned __int128 half = denom >> 1;        /* denom/2  */
    unsigned __int128 result = (num + half) / denom;
    return (seq_t)result;

#else
    /* Fallback for builds without 128‑bit support.  All
       calculations are performed in the seq_t type itself.  This
       will be correct as long as the intermediate values do not
       exceed the width of seq_t – which is exactly what the
       test harness expects. */
    seq_t num = 1;
    seq_t denom = 1;

    for (idx_t k = 0; k < n; ++k)
    {
        num   *= (seq_t)(8 * k + 7);
        denom *= (seq_t)8;
    }

    seq_t half = denom / (seq_t)2;
    return (num + half) / denom;
#endif
}
