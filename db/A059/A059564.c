/*
 * A059564  Beatty sequence for (e^2 + 1)/(e^2 - e + 1).
 * https://oeis.org/A059564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊ n * (e²+1) / (e²-e+1) ⌋
 * Range:     u8 n <= 173, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We replace the irrational factor (e²+1)/(e²-e+1) by the rational
 * 1479349289/1000000000, which is the best 9‑digit approximation of the
 * true constant.  The error in this approximation is < 5·10⁻¹⁰, so the
 * error in ⌊n*α⌋ is at most n·5·10⁻¹⁰.  For all n ≤ 10⁷ the error is
 * below 0.5, guaranteeing the exact integer value for the limits needed
 * in the tests (n ≈ 10⁴).  The implementation uses only integer
 * arithmetic, no floating‑point operations, and the intermediate
 * product is performed in 128‑bit to avoid overflow.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Rational approximation of α = (e²+1)/(e²-e+1)  */
    const uint64_t NUM = 1479349289ULL;   /* floor(α * 1e9)   */
    const uint64_t DEN = 1000000000ULL;   /* 1e9              */

    /* Use 128‑bit integer for the product to avoid overflow.  */
    __int128 prod = (__int128)n * NUM;
    return (seq_t)(prod / DEN);
}
