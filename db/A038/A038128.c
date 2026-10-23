/*
 * A038128  Beatty sequence for Euler's constant (A001620).
 * https://oeis.org/A038128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*Euler's constant)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 9885, u128 n <= 9885,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fixed-point representation of Euler's constant.
       γ ≈ P / Q with Q = 10^18 and P = floor(γ * Q). */
    const seq_t NUM = 577215664901532860ULL;          /* floor(γ * 10^18) */
    const seq_t DEN = 1000000000000000000ULL;         /* 10^18 */

    /* Perform the multiplication in 128-bit to avoid overflow. */
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)NUM;
    return (seq_t)(prod / DEN);
}
