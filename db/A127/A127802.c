/*
 * A127802  a(0) = 1, a(n) = 3*A036987(n), n>1.
 * https://oeis.org/A127802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, for n>1: a(n) = 3 if n+1 is a power of 2,
 *            otherwise 0
 * Range:     u8 n <= 830, u16 n <= 8742, u32 n <= 8742, u64 n <= 8742,
 *            u128 n <= 8742, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the nth term of OEIS A127802. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    /* n > 1 */
    /* Test whether n+1 is a power of two. */
    seq_t x = (seq_t)(n + 1);
    if ((x & (x - 1)) == 0)
        return (seq_t)3;   /* A036987(n) = 1, so 3*A036987(n) = 3 */
    else
        return (seq_t)0;   /* A036987(n) = 0 */
}
