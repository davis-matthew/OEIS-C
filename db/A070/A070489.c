/*
 * A070489  a(n) = n^3 mod 27.
 * https://oeis.org/A070489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 27
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 27 first to keep intermediate values small */
    seq_t nmod = (seq_t)(n % 27);
    seq_t r = nmod;
    r = (r * r) % 27;            /* r = n^2 mod 27 */
    r = (r * nmod) % 27;         /* r = n^3 mod 27 */
    return r;
}
