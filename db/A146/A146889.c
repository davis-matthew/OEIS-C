/*
 * A146889  Weight distribution of [12,6,4] double circulant code.
 * https://oeis.org/A146889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in 1 + 15x^2 + 32x^3 + 15x^4 + x^6
 *            (all other coefficients are 0). This is the weight distribution
 *            of the [12,6,4] double circulant code
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0 ≤ n ≤ 12 are the only non‑zero coefficients. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)32;
    if (n == 4) return (seq_t)15;
    if (n == 5) return (seq_t)0;
    if (n == 6) return (seq_t)1;
    /* All remaining indices are zero. */
    return (seq_t)0;
}
