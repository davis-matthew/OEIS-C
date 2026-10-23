/*
 * A115180  Beatty sequence for (Champernowne constant)*10 =
 *          1.234567891011121314....
 * https://oeis.org/A115180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * 1.234567891011121314...)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 12 decimal digits after the point are sufficient for n up to at least 10^6
   (error < 1 for n * 10^-12).  The scaled value ALPHA_SCALED = 1.234567891011 * 10^12.
   SCALE = 10^12.
   The following constants are cast to seq_t, so they work for all supported seq_t types. */
static const seq_t ALPHA_SCALED = (seq_t)1234567891011ULL; /* 1.234567891011 * 10^12 */
static const seq_t SCALE          = (seq_t)1000000000000ULL; /* 10^12 */

seq_t a(idx_t n)
{
    /* Compute floor(n * alpha) using integer arithmetic. */
    seq_t prod = (seq_t)n * ALPHA_SCALED;
    return prod / SCALE;
}
