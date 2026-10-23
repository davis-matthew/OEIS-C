/*
 * A014776  Inverse of 767th cyclotomic polynomial.
 * https://oeis.org/A014776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(n/767)+1 * + floor((n-72)/767)+1 (if n ≥ 72) * –
 *            floor((n-13)/767)-1 (if n ≥ 13) * – floor((n-59)/767)-1 (if n ≥
 *            59). * * This follows from 1/Φ_767(x) =
 *            (1−x)⁻¹·(1−x¹³)·(1−x⁵⁹)·(1−x⁷⁶⁷)⁻¹, * whose coefficient sequence
 *            is periodic with period 767
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t P = 767;
    /* term for j = 0 (coefficient +1) */
    seq_t term0 = (seq_t)(n / P) + 1;

    /* term for j = 13 (coefficient –1) */
    seq_t term13 = 0;
    if (n >= 13) {
        term13 = (seq_t)((n - 13u) / P) + 1;
    }

    /* term for j = 59 (coefficient –1) */
    seq_t term59 = 0;
    if (n >= 59) {
        term59 = (seq_t)((n - 59u) / P) + 1;
    }

    /* term for j = 72 (coefficient +1) */
    seq_t term72 = 0;
    if (n >= 72) {
        term72 = (seq_t)((n - 72u) / P) + 1;
    }

    /* Combine the weighted counts */
    return term0 + term72 - term13 - term59;
}
