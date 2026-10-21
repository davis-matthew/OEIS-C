/*
 * A070482  a(n) = n^3 mod 20.
 * https://oeis.org/A070482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 20
 * Range:     u8 n <= 330, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 20 first to keep intermediate values tiny. */
    seq_t m  = (seq_t)n % 20;
    seq_t m2 = (m * m) % 20;
    seq_t m3 = (m2 * m) % 20;
    return m3;
}
