/*
 * A096320  a(n) = (n^2+n+4)/2, modulo 10.
 * https://oeis.org/A096320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (n^2 + n + 4)/2 mod 10
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index mod 20 to avoid overflow on small seq_t types. */
    idx_t m = n % 20;
    seq_t tri = ((seq_t)m * ((seq_t)m + 1)) / 2;   /* triangular number modulo 10 */
    return (tri + 2) % 10;
}
