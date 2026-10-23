/*
 * A008458  Coordination sequence for hexagonal lattice.
 * https://oeis.org/A008458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 if n = 0; otherwise a(n) = 6*n
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* Use idx_t → seq_t conversion to avoid intermediate overflow on 64‑bit types */
    return (seq_t)n * (seq_t)6;
}
