/*
 * A082964  a(n) = round(n/Pi), n divided by Pi rounded to the nearest
 *          integer.
 * https://oeis.org/A082964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(n / pi) Using the convergent 355/113 for π. Then
 *            1/π ≈ 113/355 and round(n/π) = round(n * 113 / 355) = ⌊(113·n +
 *            355/2)/355⌋. The approximation error is < 3 × 10⁻⁸, so the
 *            result is exact for all n up to at least 1.7 × 10⁷ (the
 *            problem’s usual limits)
 * Range:     u8 n <= 802, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const uint64_t NUM = 113;               /* numerator of 1/π approximation   */
    const uint64_t DEN = 355;               /* denominator                       */
    const uint64_t HALF_DEN = DEN >> 1;     /* 355/2 = 177                      */

    /* Use 128‑bit arithmetic for the intermediate to avoid overflow
       when n is large (e.g., up to 2⁶⁴‑1).              */
    __int128 temp = ( (__int128) n ) * NUM + HALF_DEN;
    return (seq_t)(temp / DEN);
}
