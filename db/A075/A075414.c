/*
 * A075414  Squares of A002279: a(n) = (5*(10^n - 1)/9)^2.
 * https://oeis.org/A075414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*(10^n - 1)/9)^2 Recurrence: a(0)=0, a(1)=25,
 *            a(2)=3025, a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3) (n>2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)3025;

    /* Keep three most recent terms */
    seq_t a0 = (seq_t)0;       /* a(n-3)  */
    seq_t a1 = (seq_t)25;      /* a(n-2)  */
    seq_t a2 = (seq_t)3025;    /* a(n-1)  */
    seq_t an = 0;              /* a(n)    */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)111 * a2 - (seq_t)1110 * a1 + (seq_t)1000 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
