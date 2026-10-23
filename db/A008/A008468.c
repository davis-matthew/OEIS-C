/*
 * A008468  a(n) = n OR n^3 (applied to binary expansions).
 * https://oeis.org/A008468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = n OR n³ (binary OR)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n³ in a 64‑bit intermediate to avoid overflow */
    idx_t cube = n * n * n;
    /* Bitwise OR of n and its cube, cast to the sequence type */
    return (seq_t)n | (seq_t)cube;
}
