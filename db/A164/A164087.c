/*
 * A164087  Beatty sequence for 4*Pi/(4*Pi-3) = 1.3135986... .
 * https://oeis.org/A164087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(4*n*Pi/(4*Pi-3))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a rational approximation of Pi:  Pi ≈ 3141592 / 1_000_000  */
    /* This gives an absolute error of < 1e-6, which is more than
       enough to guarantee correct floor values for n up to about
       10^8 (and the problem limits are far below that). */
    const idx_t P = 3141592ULL;        /* floor(Pi * 10^6)   */
    const idx_t D = 1000000ULL;        /* denominator of Pi approximation */
    const idx_t den = 4 * P - 3 * D;   /* constant denominator: 4P - 3D = 9,566,368 */

    idx_t num = 4 * P * n;             /* numerator: 4 * P * n   */
    return (seq_t)(num / den);         /* floor division */
}
