/*
 * A070591  a(n) = n^4 mod 48.
 * https://oeis.org/A070591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 48
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 48 first to avoid overflow */
    seq_t v = (seq_t)n % (seq_t)48;

    /* Compute v^4 modulo 48 in two squarings */
    seq_t v2 = (v * v) % (seq_t)48;
    seq_t v4 = (v2 * v2) % (seq_t)48;

    return v4;
}
