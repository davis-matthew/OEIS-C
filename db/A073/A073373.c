/*
 * A073373  Third convolution of A001045(n+1) (generalized (1,2)-Fibonacci),
 *          n>=0, with itself.
 * https://oeis.org/A073373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            4*a(n-1)+2*a(n-2)-20*a(n-3)-a(n-4)+40*a(n-5)+8*a(n-6)-32*a(n-7)-16*a(n-8),
 *            * with a(0)=1, a(1)=4, a(2)=18, a(3)=60, a(4)=195, a(5)=576,
 *            a(6)=1644, a(7)=4488
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 22, u64 n <= 51, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)18;
    if (n == 3) return (seq_t)60;
    if (n == 4) return (seq_t)195;
    if (n == 5) return (seq_t)576;
    if (n == 6) return (seq_t)1644;
    if (n == 7) return (seq_t)4488;

    seq_t t0 = (seq_t)1;      /* a(n-8) */
    seq_t t1 = (seq_t)4;      /* a(n-7) */
    seq_t t2 = (seq_t)18;     /* a(n-6) */
    seq_t t3 = (seq_t)60;     /* a(n-5) */
    seq_t t4 = (seq_t)195;    /* a(n-4) */
    seq_t t5 = (seq_t)576;    /* a(n-3) */
    seq_t t6 = (seq_t)1644;   /* a(n-2) */
    seq_t t7 = (seq_t)4488;   /* a(n-1) */

    for (idx_t i = 8; i <= n; ++i) {
        /* Compute next term using only unsigned arithmetic */
        seq_t sumPos = 4*t7 + 2*t6 + 40*t3 + 8*t2;
        seq_t sumNeg = 20*t5 + t4 + 32*t1 + 16*t0;
        seq_t next   = sumPos - sumNeg;

        /* Shift the window to the right */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = t6;
        t6 = t7;
        t7 = next;
    }
    return t7;
}
