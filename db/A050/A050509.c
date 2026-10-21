/*
 * A050509  House numbers (version 2): a(n) = (n+1)^3 + (n+1)*Sum_{i=0..n} i.
 * https://oeis.org/A050509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/2)*(3*n+2)*(n+1)^2
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 1281, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* n as seq_t for arithmetic */
    seq_t term = (seq_t)n * 3 + 2;  /* (3*n+2) */
    term *= (nn + 1);              /* multiply by (n+1) */
    term *= (nn + 1);              /* multiply by another (n+1) */
    term /= 2;                     /* divide by 2 */
    return term;
}
