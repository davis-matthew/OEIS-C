/*
 * A168056  Expansion of (1+2*x^2+x^3)/((1-x)^2*(1+x+x^2)).
 * https://oeis.org/A168056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (12*n+3+6*cos(2*n*Pi/3)-2*sqrt(3)*sin(2*n*Pi/3))/9
 * Range:     u8 n <= 191, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168056: a(0)=1, a(1)=1, a(2)=3, a(3)=5 and for n>=4
   a(n) = a(n-1) + a(n-3) - a(n-4). */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 5;

    /* Sliding window variables: a0=a(n-4), a1=a(n-3), a2=a(n-2), a3=a(n-1) */
    seq_t a0 = 1;   /* a(0)  */
    seq_t a1 = 1;   /* a(1)  */
    seq_t a2 = 3;   /* a(2)  */
    seq_t a3 = 5;   /* a(3)  */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 + a1 - a0;   /* a(i) = a(i-1) + a(i-3) - a(i-4) */
        a0 = a1;             /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;  /* a(n) is in a3 after loop */
}
