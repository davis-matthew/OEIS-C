/*
 * A070506  a(n) = n^3 mod 44.
 * https://oeis.org/A070506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n^3 mod 44
   a(0) = 0, a(1) = 1, a(2) = 8, ...
   The value is computed by reducing the index modulo 44 first, then
   taking the cube modulo 44 using only integer arithmetic. */
seq_t a(idx_t n)
{
    /* Reduce n modulo 44 */
    unsigned nmod = (unsigned)(n % 44);

    /* Compute (nmod^2) mod 44 while staying in a safe integer type */
    unsigned sq = (unsigned)((nmod * nmod) % 44);

    /* Compute (sq * nmod) mod 44: this is n^3 mod 44 */
    unsigned res = (unsigned)((sq * nmod) % 44);

    return (seq_t)res;
}
