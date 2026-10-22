/*
 * A000004  The zero sequence.
 * https://oeis.org/A000004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, easy, nonn, mult
 * Formula:   a(n) = 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    (void)n;
    return (seq_t)0;
}
