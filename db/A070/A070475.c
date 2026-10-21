/*
 * A070475  a(n) = n^3 mod 13.
 * https://oeis.org/A070475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 13
 * Range:     u8 n <= 277, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 13 before cubing to keep intermediate values small. */
    seq_t m = (seq_t)n % 13;          /* m ∈ [0,12] */
    seq_t sq = (m * m) % 13;          /* m^2 mod 13 */
    seq_t cube = (sq * m) % 13;       /* m^3 mod 13 */
    return cube;
}
