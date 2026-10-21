/*
 * A091142  a(n) = 2*a(n-1) + 4*a(n-2) - 2*a(n-3) with initial terms 1, 2, 6.
 * https://oeis.org/A091142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 2*a(n-3) (a(1)=1, a(2)=2, a(3)=6)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return the first three terms explicitly */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    if (n == (idx_t)3) return (seq_t)6;

    /* iterate the recurrence */
    seq_t a1 = (seq_t)1;   /* a(n-3)  */
    seq_t a2 = (seq_t)2;   /* a(n-2)  */
    seq_t a3 = (seq_t)6;   /* a(n-1)  */
    seq_t an;              /* a(n)    */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        an = (seq_t)2 * a3 + (seq_t)4 * a2 - (seq_t)2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
