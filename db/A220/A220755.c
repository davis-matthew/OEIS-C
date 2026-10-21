/*
 * A220755  Numbers n such that n^2 + n(n+1)/2 is an oblong number (A002378).
 * https://oeis.org/A220755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 98*a(n-2) - 98*a(n-3) - a(n-4) + a(n-5); a(1) =
 *            0, a(2) = 1, a(3) = 28, a(4) = 117, a(5) = 2760;
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices above the offset */
    if (n == 0) return (seq_t)0;

    /* Directly return the first five known terms. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)28;
    if (n == 4) return (seq_t)117;
    if (n == 5) return (seq_t)2760;

    /* Sliding window for the last five computed terms:
       t1 = a(k-5), t2 = a(k-4), t3 = a(k-3), t4 = a(k-2), t5 = a(k-1)   */
    seq_t t1 = (seq_t)0;      /* a(1)  */
    seq_t t2 = (seq_t)1;      /* a(2)  */
    seq_t t3 = (seq_t)28;     /* a(3)  */
    seq_t t4 = (seq_t)117;    /* a(4)  */
    seq_t t5 = (seq_t)2760;   /* a(5)  */

    seq_t next;
    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = a(i-1) + 98*a(i-2) - 98*a(i-3) - a(i-4) + a(i-5) */
        next = t5
             + t4 * (seq_t)98u
             - t3 * (seq_t)98u
             - t2
             + t1;

        /* Shift the window to include the new term. */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = next;
    }
    return t5;
}
