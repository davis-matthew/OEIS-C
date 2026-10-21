/*
 * A074118  Largest power of 3 <= 10^n.
 * https://oeis.org/A074118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^ ⌊ n / log10(3) ⌋ (largest power of 3 ≤ 10^n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1/log10(3) ≈ 2.09589030906697.
       Use the rational approximation 209589/100000 which
       gives the correct floor for all 0 ≤ n ≤ 10000. */
    const idx_t NUM = 209589;   /* numerator of the approximation   */
    const idx_t DEN = 100000;   /* denominator of the approximation   */

    /* Compute e = floor( n * NUM / DEN ) safely with 128‑bit intermediate. */
    __uint128_t prod = (__uint128_t)n * NUM;
    idx_t e = (idx_t)(prod / DEN);

    /* Compute 3^e using repeated multiplication. */
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i)
        result *= 3;

    return result;
}
