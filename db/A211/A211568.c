/*
 * A211568  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          three or four distinct values for every i,j,k<=n.
 * https://oeis.org/A211568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4*a(n-1)+a(n-2)-21*a(n-3)+16*a(n-4)+29*a(n-5)-34*a(n-6)-6*a(n-7)+12*a(n-8)
 *            Initial values (n=1..8): 24, 44, 78, 142, 256, 472, 868, 1626
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first eight terms directly */
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)44;
    if (n == 3) return (seq_t)78;
    if (n == 4) return (seq_t)142;
    if (n == 5) return (seq_t)256;
    if (n == 6) return (seq_t)472;
    if (n == 7) return (seq_t)868;
    if (n == 8) return (seq_t)1626;

    /* Sliding window variables: a(n-8) ... a(n-1) */
    seq_t a8 = (seq_t)24;   /* a(n-8) */
    seq_t a7 = (seq_t)44;   /* a(n-7) */
    seq_t a6 = (seq_t)78;   /* a(n-6) */
    seq_t a5 = (seq_t)142;  /* a(n-5) */
    seq_t a4 = (seq_t)256;  /* a(n-4) */
    seq_t a3 = (seq_t)472;  /* a(n-3) */
    seq_t a2 = (seq_t)868;  /* a(n-2) */
    seq_t a1 = (seq_t)1626; /* a(n-1) */

    seq_t val = a1; /* placeholder */

    /* Compute successive terms up to n */
    for (idx_t i = 9; i <= n; ++i) {
        val = (seq_t)4 * a1
            +          a2
            - (seq_t)21 * a3
            + (seq_t)16 * a4
            + (seq_t)29 * a5
            - (seq_t)34 * a6
            - (seq_t)6  * a7
            + (seq_t)12 * a8;

        /* shift window */
        a8 = a7;
        a7 = a6;
        a6 = a5;
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = val;
    }

    return val;
}
