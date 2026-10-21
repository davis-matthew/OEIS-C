/*
 * A061654  a(n) = (3*16^n + 2)/5.
 * https://oeis.org/A061654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*16^n + 2)/5 Alternative recurrence: a(0)=1, a(1)=10,
 *            a(n)=17*a(n-1)-16*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* Iteratively apply the recurrence.  All operations are performed on
       seq_t values; small integer constants are cast to seq_t before
       arithmetic to keep everything in the proper type. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)10;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 * (seq_t)17 - prev2 * (seq_t)16;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
