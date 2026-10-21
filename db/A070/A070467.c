/*
 * A070467  a(n) = n^2 mod 45.
 * https://oeis.org/A070467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n mod 45
 * Range:     u8 n <= 283, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 45 first to keep intermediates small */
    seq_t nmod = (seq_t)n % 45;
    /* Compute square and reduce modulo 45 */
    return (nmod * nmod) % 45;
}
