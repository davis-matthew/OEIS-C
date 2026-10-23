/*
 * A168187  a(n) = n^3*(n^6 + 1)/2.
 * https://oeis.org/A168187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3*(n^6 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 149, u128 n <= 400,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n6 = n3 * n3;           /* n^6 */

    /* One of n^3 and (n^6 + 1) is even, so we can divide first
       to keep intermediate values smaller if seq_t is small.   */
    if ((nn & 1) == 0) {          /* n even -> n^3 even */
        return (n3 / 2) * (n6 + (seq_t)1);
    } else {                      /* n odd -> n^6+1 even */
        return n3 * ((n6 + (seq_t)1) / 2);
    }
}
