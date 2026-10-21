/*
 * A070507  a(n) = n^3 mod 45.
 * https://oeis.org/A070507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 45
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 45 to avoid overflow, then cube and take mod 45 again. */
    seq_t m = ((seq_t)n) % (seq_t)45;
    seq_t cube = m * m * m;
    return cube % (seq_t)45;
}
