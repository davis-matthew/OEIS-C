/*
 * A059553  Beatty sequence for Gamma(2/3).
 * https://oeis.org/A059553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * Γ(2/3)) (Γ is the Gamma function)
 * Range:     u8 n <= 189, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Gamma(2/3) ≈ 1.35411793942640046448…  (20 decimal digits)   */
    static const unsigned __int128 NUM =
        ((unsigned __int128)7 << 64) | (unsigned __int128)6284585427673081216ULL; /* numerator */
    static const unsigned __int128 DEN =
        ((unsigned __int128)5 << 64) | (unsigned __int128)7766279631452241920ULL; /* denominator = 10^20 */

    unsigned __int128 prod = (unsigned __int128)n * NUM;
    return (seq_t)(prod / DEN);
}
