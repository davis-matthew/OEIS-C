/*
 * A081274  Duplicate of A038764.
 * https://oeis.org/A081274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (9*n^2 + 3*n + 2) / 2
 * Range:     i8 n <= 3, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* cast index to seq_t first */
    seq_t val = ((seq_t)9 * nn * nn)      /* 9*n^2 */
              + ((seq_t)3 * nn)          /* + 3*n */
              + (seq_t)2;                /* + 2 */
    return val / (seq_t)2;               /* divide by 2 */
}
