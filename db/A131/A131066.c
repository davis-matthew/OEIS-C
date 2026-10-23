/*
 * A131066  Binomial transform of [1, 1, 6, 6, 6, ...].
 * https://oeis.org/A131066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*2^n - 5*(n + 1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow *= (seq_t)2;          /* multiply by 2 */
    }

    /* Compute 6*2^n */
    seq_t val = pow * (seq_t)6;

    /* Compute 5*(n + 1) */
    seq_t sub = (seq_t)n + (seq_t)1;
    sub *= (seq_t)5;

    return val - sub;          /* subtraction is safe for all non‑negative n */
}
