/*
 * A062075  a(n) = n^4 * 4^n.
 * https://oeis.org/A062075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 * 4^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A062075: a(n) = n^4 * 4^n for n >= 0. */
seq_t a(idx_t n)
{
    /* Compute 4^n */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= 4;

    /* Compute n^4 */
    seq_t t = (seq_t)n;
    t *= t;  /* n^2 */
    t *= t;  /* n^4 */

    return pow4 * t;
}
