/*
 * A164897  a(n) = 4*n*(n+1) + 3.
 * https://oeis.org/A164897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(n+1) + 3
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 900, u64 n <= 900,
 *            u128 n <= 900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* convert index to seq_t */
    seq_t val = tn * (tn + (seq_t)1);  /* n*(n+1) */
    val = val * (seq_t)4;         /* 4*n*(n+1) */
    val += (seq_t)3;              /* +3 */
    return val;
}
