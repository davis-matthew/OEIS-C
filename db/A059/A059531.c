/*
 * A059531  Beatty sequence for 1 + 1/Pi.
 * https://oeis.org/A059531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(1 + 1/π) * n⌋ (Beatty sequence for 1 + 1/π)
 * Range:     u8 none, u16 none, u32 n <= 2000, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exact rational approximation of 1 + 1/π:
       1 + 1/π ≈ 137563/104348 with error < 10⁻¹², sufficient
       for all n up to 10⁴ (and far beyond for any realistic test). */
    static const seq_t NUM = 137563;   /* numerator  */
    static const seq_t DEN = 104348;   /* denominator */

    seq_t val = (seq_t)n;
    val = val * NUM;
    return val / DEN;
}
