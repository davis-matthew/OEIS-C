/*
 * A059545  Beatty sequence for log(10).
 * https://oeis.org/A059545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*log(10))
 * Range:     u8 n <= 111, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* High‑precision rational approximation of ln(10):
       ln(10) ≈ 2302585092994045684 / 1000000000000000000  (18 decimal digits) */
    static const unsigned __int128 NUM =
        (unsigned __int128)2302585092994045684ULL;      /* numerator */
    static const unsigned __int128 DEN =
        (unsigned __int128)1000000000000000000ULL;      /* denominator */

    /* Compute floor(n * NUM / DEN) using 128‑bit arithmetic. */
    unsigned __int128 product = (unsigned __int128)n * NUM;
    unsigned __int128 result  = product / DEN;

    return (seq_t)result;
}
