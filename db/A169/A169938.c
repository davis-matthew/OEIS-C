/*
 * A169938  a(n) = n*(n+1)*(n*(n+1)+1).
 * https://oeis.org/A169938
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n*(n+1)+1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < -1)               /* undefined indices become 0 */
        return 0;
    if (n == -1)              /* first term of the sequence */
        return 0;

    seq_t tn = (seq_t)n;      /* cast index to the arithmetic type */
    seq_t t1 = tn * (tn + 1); /* t1 = n * (n + 1) */
    return t1 * (t1 + 1);     /* a(n) = t1 * (t1 + 1) */
}
