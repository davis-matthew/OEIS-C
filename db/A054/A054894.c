/*
 * A054894  a(n+1) = 4*a(n) + 4*a(n-1) - 4*a(n-2) - a(n-3) with a(1)=1,
 *          a(2)=2, a(3)=11, a(4)=48.
 * https://oeis.org/A054894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) - 4*a(n-3) - a(n-4) with a(1)=1,
 *            a(2)=2, a(3)=11, a(4)=48
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)48;

    /* Use only the last four values, iterating up to n */
    seq_t t1 = (seq_t)1;   /* a(n-4) */
    seq_t t2 = (seq_t)2;   /* a(n-3) */
    seq_t t3 = (seq_t)11;  /* a(n-2) */
    seq_t t4 = (seq_t)48;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)4 * t4 + (seq_t)4 * t3 - (seq_t)4 * t2 - t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }
    return t4;
}
