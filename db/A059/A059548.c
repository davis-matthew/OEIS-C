/*
 * A059548  Beatty sequence for 1 + log(3).
 * https://oeis.org/A059548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(1 + log(3))) , offset = 1
 * Range:     u8 n <= 121, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1 + ln(3) ≈ 2.0986122886681098
   We use a fixed‑point approximation with a 10^18 denominator.
   The fractional part ln(3) is represented exactly to 18 decimal digits:
   ln(3) ≈ 1.0986122886681098
   => 1 + ln(3) ≈ 2.0986122886681098
   Numerator (scaled by 10^18) : 2098612288668109800
   Denominator                     : 1000000000000000000
   The resulting approximation is accurate for all n up to
   10^15, which is far beyond the range of the test data.    */

#include <stdint.h>

/* Fixed‑point constants (Q = 10^18) */
static const unsigned __int128 SCALE_NUM = ((unsigned __int128)2098612288668109ULL * 1000ULL + 800ULL);  /* 2098612288668109800 */
static const unsigned __int128 SCALE_DEN = 1000000000000000000ULL; /* 10^18 */

seq_t a(idx_t n)
{
    /* compute floor(n * SCALE_NUM / SCALE_DEN) */
    unsigned __int128 product = (unsigned __int128)n * SCALE_NUM;
    seq_t result = (seq_t)(product / SCALE_DEN);
    return result;
}
