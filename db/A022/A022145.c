/*
 * A022145  Coordination sequence for root lattice B_3.
 * https://oeis.org/A022145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 20*n^2 - 4*n + 2 (for n > 0)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sn = (seq_t)n;
    return ((seq_t)20) * sn * sn - ((seq_t)4) * sn + ((seq_t)2);
}
