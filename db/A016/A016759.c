/*
 * A016759  a(n) = (2*n + 1)^7.
 * https://oeis.org/A016759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)^7
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 11, u64 n <= 282,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t odd = ((seq_t)n * 2) + 1;
    seq_t r1   = odd * odd;     /* (2n+1)^2 */
    seq_t r2   = r1 * r1;       /* (2n+1)^4 */
    seq_t r3   = r2 * odd;      /* (2n+1)^5 */
    seq_t r4   = r3 * odd;      /* (2n+1)^6 */
    seq_t r5   = r4 * odd;      /* (2n+1)^7 */
    return r5;
}
