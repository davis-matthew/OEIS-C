/*
 * A194210  a(n) = floor(Sum_{k=1..n} <k*Pi/3>), where < > denotes fractional
 *          part.
 * https://oeis.org/A194210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ Σ_{k=1..n} frac(k*Π/3) ⌋ where frac(x) = x - ⌊x⌋
 *            denotes the fractional part. The computation is performed using
 *            a fixed‑point approximation of the fractional part of Π/3 with
 *            48 fractional bits
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* 64‑bit constants for the fixed‑point conversion.
       We approximate Π by floor(Π*10¹⁶) / 10¹⁶.
       The error is < 10⁻¹⁶, sufficiently small for the chosen 48
       fractional bits (error on the final result < 10⁻³). */
    static const unsigned long long SCALE   = (unsigned long long)1 << 48;   /* 2^48 */
    static const unsigned long long PI_SCALE = 31415926535897932ULL;         /* floor(Π*10¹⁶) */
    static const unsigned long long Q       = 10000000000000000ULL;          /* 10¹⁶ */

    /* C = floor( (Π-3)/3 * SCALE ) is computed once using 128‑bit arithmetic. */
    static const unsigned long long C = (unsigned long long)(
        ((unsigned __int128)(PI_SCALE - 3 * Q) * SCALE) / (3 * Q));

    unsigned __int128 r         = 0;          /* current fractional part (scaled)  */
    unsigned __int128 sum_frac  = 0;          /* accumulated fractional sum (scaled) */
    seq_t          acc         = 0;          /* integer part of the result */

    for (idx_t k = 1; k <= n; ++k) {
        r += C;
        if (r >= SCALE)
            r -= SCALE;            /* r = (r + C) mod SCALE */

        sum_frac += r;
        if (sum_frac >= SCALE) {   /* sum_frac has overflowed one unit */
            acc++;
            sum_frac -= SCALE;
        }
    }
    return acc;
}
