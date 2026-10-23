/*
 * A109632  In the game of bridge, a(n) is the penalty for going down n tricks
 *          in a vulnerable, doubled contract.
 * https://oeis.org/A109632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 300n - 100
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n) * ((seq_t)300) - ((seq_t)100);
}
