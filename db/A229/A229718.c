/*
 * A229718  Number of arrays of length 2 that are sums of n consecutive
 *          elements of length 2+n-1 permutations of 0..2+n-2, and no two
 *          consecutive rises or falls in the latter permutation.
 * https://oeis.org/A229718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n > 5 Initial terms:
 *            a(1)=2, a(2)=4, a(3)=8, a(4)=18, a(5)=26
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n<1, return 0 */

    /* Direct returns for the first five terms */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)18;
    if (n == 5) return (seq_t)26;

    /* Let a1..a5 hold a(i-5) .. a(i-1) respectively for the
       current iteration value of i.  Initially i = 6. */
    seq_t a1 = (seq_t)2;      /* a(1) */
    seq_t a2 = (seq_t)4;      /* a(2) */
    seq_t a3 = (seq_t)8;      /* a(3) */
    seq_t a4 = (seq_t)18;     /* a(4) */
    seq_t a5 = (seq_t)26;     /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - 2*a(i-3) + a(i-4) */
        seq_t ai = ( (seq_t)2 * a5 ) - ( (seq_t)2 * a3 ) + a2;

        /* Shift the window for the next iteration:
           a1<-a2, a2<-a3, a3<-a4, a4<-a5, a5<-ai */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = ai;
    }
    return a5;   /* a5 now holds a(n) for i==n */
}
