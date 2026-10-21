/*
 * A087957  a(n) is the square of the n-th partial sum minus the n-th partial
 *          sum of the squares, divided by a(n-1), for all n>=1, starting with
 *          a(0)=1, a(1)=4.
 * https://oeis.org/A087957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3); initial values: a(0)=1,
 *            a(1)=4, a(2)=2, a(3)=14
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)14;

    /* Use three working variables to generate the sequence on the fly. */
    seq_t a0 = (seq_t)1;   /* a(k-3)  */
    seq_t a1 = (seq_t)4;   /* a(k-2)  */
    seq_t a2 = (seq_t)2;   /* a(k-1)  */
    seq_t a3 = (seq_t)14;  /* a(k)    */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* next = a(k) + 3*a(k-1) - a(k-2)  */
        next = a3 + (seq_t)3 * a2 - a1;

        /* shift the three last terms for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
