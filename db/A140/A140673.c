/*
 * A140673  a(n) = 3*n*(n + 5)/2.
 * https://oeis.org/A140673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(n + 5)/2
 * Range:     u8 n <= 7, u16 n <= 205, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nx = n;                 /* convert index to sequence type */
    seq_t val = nx * (nx + 5);    /* n*(n+5) */
    val *= 3;                     /* times 3 */
    val /= 2;                     /* divide by 2 (exact) */
    return val;
}
