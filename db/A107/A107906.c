/*
 * A107906  Expansion of (1+8*x)/(1-16*x^2).
 * https://oeis.org/A107906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2n + (n mod 2)) (OEIS A107906)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1, a(1)=8, a(2)=16, a(3)=128, a(4)=256, ... */

#include <stdint.h>

/* seq_t and idx_t are defined by the build environment.
   They are unsigned integer types of various bit widths
   (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128,
   or a big‑integer class in the arbitrary‑precision build). */

seq_t a(idx_t n)
{
    /* Compute the exponent e = 2*n + (n & 1)   */
    idx_t exp = (n << 1) + (n & 1);   /* 2*n + (n mod 2) */

    /* Compute 2^exp by repeated left shifts.
       Using a simple loop keeps the code valid for all
       unsigned seq_t types (overflow is defined modulo). */
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result <<= 1;
    }

    return result;
}
