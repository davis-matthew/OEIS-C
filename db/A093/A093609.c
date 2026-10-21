/*
 * A093609  Upper Beatty sequence for e^G, G = Euler's gamma constant.
 * https://oeis.org/A093609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * (2.781072417990197...)) where the multiplier
 *            2.781072417990197 = (k + 1) with k = e^γ ≈ 1.781072417990197
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* We use a 64‑bit rational approximation that is accurate for all n
   up to at least 10 000, which is the range required for the tests.
   The denominator 10^18 is chosen so that rounding the product n * NUM
   does not change the integer part of the division. */

/* Multiplier numerator rounded to nearest integer: 2.781072417990197 × 10^18 */
static const uint64_t NUM = 2781072417990197000ULL;
/* Denominator: 10^18 */
static const uint64_t DEN = 1000000000000000000ULL;

/* Compute floor(n * (NUM/DEN)) using 128‑bit arithmetic for safety. */
seq_t a(idx_t n)
{
    /* 128‑bit product to avoid overflow for n up to 10 000 */
    __int128 prod = (__int128)n * NUM;
    seq_t val = (seq_t)(prod / DEN);
    return val;
}
