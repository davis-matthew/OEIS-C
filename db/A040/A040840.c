/*
 * A040840  Continued fraction for sqrt(870).
 * https://oeis.org/A040840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 29; for n ≥ 1, a(n) = (n is odd) ? 2 : 58
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This produces the continued‑fraction coefficients of √870. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;
    return (n & 1) ? (seq_t)2 : (seq_t)58;
}
