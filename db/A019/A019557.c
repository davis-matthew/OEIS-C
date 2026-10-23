/*
 * A019557  Coordination sequence for G_2 lattice.
 * https://oeis.org/A019557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 18*n - 6 (n >= 1)
 * Range:     u8 n <= 14, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* Use seq_t arithmetic, cast n to seq_t before multiplication */
    return (seq_t)n * (seq_t)18 - (seq_t)6;
}
