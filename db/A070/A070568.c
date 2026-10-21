/*
 * A070568  a(n) = n^4 mod 25.
 * https://oeis.org/A070568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 25
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 25 before computing the fourth power to avoid overflow. */
    seq_t r  = ((seq_t)n) % 25;          /* first power modulo 25 */
    seq_t r2 = (r * r) % 25;            /* (n^2) mod 25 */
    seq_t r4 = (r2 * r2) % 25;          /* (n^4) mod 25 */
    return r4;
}
