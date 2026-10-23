/*
 * A192850  Molecular topological indices of the web graphs.
 * https://oeis.org/A192850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(6*n^2 + 22*n + 3*((-1)^n + 7))
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 893, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* convert index to value type */
    int   sgn = (n & 1) ? -1 : 1;   /* (-1)^n */
    seq_t part = 6 * nn * nn;       /* 6*n^2 */
    part += 22 * nn;                /* +22*n */
    part += 3 * (seq_t)(sgn + 7);   /* +3*((-1)^n + 7) */
    return nn * part;               /* n * part */
}
