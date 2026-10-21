/*
 * A070576  a(n) = n^4 mod 33.
 * https://oeis.org/A070576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 33
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t M = 33;
    seq_t nmod = (seq_t)n % M;        /* reduce n modulo 33 first   */
    seq_t sq   = (nmod * nmod) % M;   /* n^2 mod 33                  */
    return (sq * sq) % M;             /* n^4 mod 33                  */
}
