/*
 * A094561  Final decimal digit of floor(Pi*n), Pi=3.1415...
 * https://oeis.org/A094561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(pi * n) mod 10 * * We approximate pi by its integer
 *            representation to 15 decimal * places: S = floor(pi * 10^15) =
 *            3141592653589793. * For all n < 10^15 (the required range),
 *            floor(pi*n) equals * (S * n) / 10^15, so we can compute the
 *            value exactly using 128‑bit * arithmetic and then...
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const __int128 PI10_15 = ( __int128 )3141592653589793ULL;   /* floor(pi*1e15) */
static const __int128 BASE10_15 = ( __int128 )1000000000000000ULL; /* 1e15 */

seq_t a(idx_t n)
{
    /* Convert to 128‑bit to avoid overflow */
    __int128 prod = (__int128)PI10_15 * (__int128)n;
    __int128 val  = prod / BASE10_15;      /* floor(pi*n) */
    return (seq_t)(val % 10);              /* final decimal digit */
}
