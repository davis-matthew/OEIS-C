/*
 * A070517  a(n) = n^4 mod 13.
 * https://oeis.org/A070517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 13
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 13 first */
    seq_t r = (seq_t)(n % 13);

    /* Compute r^4 % 13 efficiently */
    r = (r * r) % 13;   /* r^2 mod 13 */
    r = (r * r) % 13;   /* (r^2)^2 = r^4 mod 13 */

    return r;
}
