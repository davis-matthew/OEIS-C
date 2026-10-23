/*
 * A131708  A024494 prefixed by a 0.
 * https://oeis.org/A131708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) Initial values: a(0)=0,
 *            a(1)=1, a(2)=2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A131708 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Sliding window of the last three values */
    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)2;   /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)3 * a2 - (seq_t)3 * a1 + (seq_t)2 * a0;
        a0 = a1;   /* shift window forward */
        a1 = a2;
        a2 = cur;
    }

    return a2; /* returns a(n) */
}
