/*
 * A152896  a=b=c=0;c(n)=c+n+a;b(n)=b+n+c;a(n)=a+n+b.
 * https://oeis.org/A152896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1)-12*a(n-2)+11*a(n-3)-5*a(n-4)+a(n-5) with initial
 *            values a(1)=0, a(2)=3, a(3)=15, a(4)=55, a(5)=183
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)55;
    if (n == 5) return (seq_t)183;

    /* Variables hold a(i-5), a(i-4), a(i-3), a(i-2), a(i-1) */
    seq_t a0 = (seq_t)0;   /* a(i-5)  */
    seq_t a1 = (seq_t)3;   /* a(i-4)  */
    seq_t a2 = (seq_t)15;  /* a(i-3)  */
    seq_t a3 = (seq_t)55;  /* a(i-2)  */
    seq_t a4 = (seq_t)183; /* a(i-1)  */

    seq_t an = 0;          /* will hold a(i) */

    for (idx_t i = 6; i <= n; ++i) {
        an = 6*a4 - 12*a3 + 11*a2 - 5*a1 + a0;
        if (i == n) return an;          /* last iteration, return result */
        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
