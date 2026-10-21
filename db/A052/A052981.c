/*
 * A052981  Expansion of ( 1-x ) / ( 1-4*x-3*x^2+3*x^3 ).
 * https://oeis.org/A052981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1)+4*a(n-2)-3*a(n-3) for n>=3, a(0)=1, a(1)=3,
 *            a(2)=15. The recurrence follows from the generating function (1
 *            - x) / (1 - 4*x - 3*x^2 + 3*x^3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three known values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)15;

    /* Use a simple iterative recurrence. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)15;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 4*a2 + 3*a1 - 3*a0 */
        seq_t next = (seq_t)4 * a2 + (seq_t)3 * a1;
        next -= (seq_t)3 * a0;
        /* Shift for next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;  /* a(n) is stored in a2 after loop */
}
