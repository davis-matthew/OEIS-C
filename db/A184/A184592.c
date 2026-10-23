/*
 * A184592  a(n) = floor((n*(Pi-1) + 1)/(Pi-2)); complement of A184591.
 * https://oeis.org/A184592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n*(Pi-1)+1)/(Pi-2))
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  The Beatty identity gives a nicer form:
     a(n) = n + floor((n+1)/(Pi-2))
   The constant Pi-2 is irrational, so we approximate it to 10⁻⁷
   precision with a fixed‑point representation:
     SCALE      = 10⁷
     PI_SCALED  = floor(Pi * SCALE) = 31415926
     C_SCALED   = (Pi-2) * SCALE   = 11415926  */
static const uint64_t SCALE     = 10000000ULL;
static const uint64_t PI_SCALED = 31415926ULL;    /* floor(π * SCALE)   */
static const uint64_t C_SCALED  = PI_SCALED - 2 * SCALE;  /* (π-2) * SCALE */

seq_t a(idx_t n)
{
    /* Compute b = floor((n+1) / (π-2)) using fixed‑point arithmetic.
       The product (n+1)*SCALE fits into 128‑bit (guaranteed for n
       in the typical OEIS test range). */
    unsigned __int128 num = (unsigned __int128)(n + 1) * SCALE;
    seq_t b = (seq_t)(num / C_SCALED);

    /* a(n) = n + b  */
    return (seq_t)n + b;
}
