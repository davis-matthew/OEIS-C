/*
 * A119997  Sum of all matrix elements of n X n matrix M[i,j] =
 *          (-1)^(i+j)*Fibonacci[i+j-1].
 * https://oeis.org/A119997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+a(n-2)-7*a(n-3)+5*a(n-4)-a(n-5) for n>5 with
 *            initial values a(1)=1, a(2)=1, a(3)=4, a(4)=5, a(5)=17
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* undefined for n<1 */

    /* handle the first five terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)5;
    if (n == 5) return (seq_t)17;

    /* iterative computation using the recurrence */
    seq_t t1 = (seq_t)1;     /* a(n-5)  */
    seq_t t2 = (seq_t)1;     /* a(n-4)  */
    seq_t t3 = (seq_t)4;     /* a(n-3)  */
    seq_t t4 = (seq_t)5;     /* a(n-2)  */
    seq_t t5 = (seq_t)17;    /* a(n-1)  */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t t6 = (seq_t)3 * t5 + t4 - (seq_t)7 * t3 + (seq_t)5 * t2 - t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = t6;
    }
    return t5;
}
