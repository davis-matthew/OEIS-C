/*
 * A070480  a(n) = n^3 mod 18.
 * https://oeis.org/A070480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 18
 * Range:     u8 n <= 272, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work modulo 18 to avoid overflow on large n. */
    seq_t nmod = (seq_t)n % 18;      /* n mod 18 */
    seq_t sq   = (nmod * nmod) % 18; /* (n^2) mod 18 */
    seq_t cub  = (sq * nmod) % 18;   /* (n^3) mod 18 */
    return cub;
}
