/*
 * A210678  a(n) = a(n-1)+a(n-2)+n+2, a(0)=a(1)=1.
 * https://oeis.org/A210678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+n+2 with a(0)=a(1)=1
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial small indices directly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* keep only the last two values; works for any seq_t that
       supports +, a default constructor and assignment. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t i_s = (seq_t)i;                 /* cast index to seq_t */
        seq_t ai = a1 + a0 + i_s + (seq_t)2;  /* recurrence */
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
