/*
 * A194208  a(n) = [Sum_{k=1..n} (k*Pi/2)], where [ ]=floor, ( )=fractional
 *          part.
 * https://oeis.org/A194208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{k=1..n} {k*Π/2} ), where {x} denotes the
 *            fractional part of x. The sequence is generated with
 *            high‑precision integer arithmetic by keeping the fractional part
 *            of Π/2 in 52‑bit fixed‑point representation. ``BASE = 2^52`` and
 *            ``ALPHA`` is the integer representation of the fractiona...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 52‑bit fixed‑point representation of the fractional part of π/2.
       The hexadecimal constant 0x921FB54442D18 is the 52‑bit fraction
       stored in a double for the value π/2 (see IEEE‑754 representation). */
    const uint64_t BASE = ((uint64_t)1) << 52;          /* 2^52          */
    const uint64_t ALPHA = 0x921fb54442d18ULL;          /* ≈ (π/2−1)·2^52 */

    uint64_t x   = 0;                                 /* fractional part ·BASE */
    unsigned __int128 sum = 0;                        /* accumulated scaled sum */

    for (idx_t k = 1; k <= n; ++k) {
        x += ALPHA;                                   /* add scaled fraction   */
        if (x >= BASE) x -= BASE;                     /* keep x < BASE         */
        sum += x;                                     /* accumulate           */
    }

    return (seq_t)(sum / BASE);                       /* floor of sum of fractions */
}
