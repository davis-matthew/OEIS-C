/*
 * A085899  a(n) = floor( 2*(1 + n + 2*n^2 + 4*n^3)/(1 + 2*n + n^2)).
 * https://oeis.org/A085899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( 2*(1 + n + 2*n^2 + 4*n^3) / (1 + 2*n + n^2) )
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t to avoid mix of types */
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;         /* n^2 */
    seq_t n3  = n2 * sn;         /* n^3 */
    seq_t num = 2
              + 2 * sn
              + 4 * n2
              + 8 * n3;           /* 2 + 2n + 4n^2 + 8n^3 */
    seq_t den = 1
              + 2 * sn
              + n2;                /* 1 + 2n + n^2 */
    return num / den;            /* integer division = floor for positive */
}
