/*
 * A157792  Least number of lattice points on one side from which every point
 *          of a square n X n lattice is visible.
 * https://oeis.org/A157792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn
 * Formula:   a(n) = ⌊(n+1)/2⌋ for n ≤ 8; a(n) = ⌈(n+1)/3⌉ for n ≥ 9
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 8u) {
        /* floor((n+1)/2) */
        return ((seq_t)n + 1) / 2;
    } else {
        /* ceil((n+1)/3) = (n+3)/3 */
        return ((seq_t)n + 3) / 3;
    }
}
