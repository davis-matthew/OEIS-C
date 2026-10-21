/*
 * A092493  a(n) = 4a(n-1) - 4a(n-2) + 3a(n-3) + a(n-4) - a(n-5).
 * https://oeis.org/A092493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4a(n-1) - 4a(n-2) + 3a(n-3) + a(n-4) - a(n-5), with
 *            offset 1: a(1)=1, a(2)=2, a(3)=5, a(4)=14, a(5)=42
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n<1, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)14;
    if (n == 5) return (seq_t)42;

    /* sliding window for the last five terms */
    seq_t t1 = (seq_t)1;   /* a(n-5) */
    seq_t t2 = (seq_t)2;   /* a(n-4) */
    seq_t t3 = (seq_t)5;   /* a(n-3) */
    seq_t t4 = (seq_t)14;  /* a(n-2) */
    seq_t t5 = (seq_t)42;  /* a(n-1) */

    seq_t current;
    for (idx_t i = 6; i <= n; ++i)
    {
        current = 4*t5 - 4*t4 + 3*t3 + t2 - t1;
        /* shift window for next iteration */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = current;
    }
    return current;
}
