/*
 * A158326  a(n) = 484*n + 1.
 * https://oeis.org/A158326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 484*n + 1
 * Range:     u8 none, u16 n <= 135, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1, but the formula works for any n. */
    return ((seq_t)n * (seq_t)484) + (seq_t)1;
}
