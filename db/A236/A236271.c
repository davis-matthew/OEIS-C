/*
 * A236271  Number of (n+1) X (1+1) 0..2 arrays with the upper median unequal
 *          to the lower median in every 2 X 2 subblock.
 * https://oeis.org/A236271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 4*a(n-2) - 16*a(n-3) - 8*a(n-4); initial
 *            values: a(1)=42, a(2)=208, a(3)=1048, a(4)=5328
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset 1; n < 1 is undefined */
    if (n == 1)
        return (seq_t)42;
    if (n == 2)
        return (seq_t)208;
    if (n == 3)
        return (seq_t)1048;
    if (n == 4)
        return (seq_t)5328;

    seq_t a1 = (seq_t)42;
    seq_t a2 = (seq_t)208;
    seq_t a3 = (seq_t)1048;
    seq_t a4 = (seq_t)5328;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t a5 = 5 * a4 + 4 * a3;    /* positive part */
        a5 -= 16 * a2;                /* subtract 16*a(n-3) */
        a5 -= 8 * a1;                 /* subtract 8*a(n-4) */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }
    return a4;
}
