/*
 * A192386  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 12*a(n-2) - 8*a(n-3) - 16*a(n-4). Initial
 *            values: a(1)=1, a(2)=0, a(3)=8, a(4)=8
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset directly. */
    if (n <= 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)8;

    /* Variables holding the last four terms. */
    seq_t a1 = (seq_t)1;  /* a(n-4)  */
    seq_t a2 = (seq_t)0;  /* a(n-3)  */
    seq_t a3 = (seq_t)8;  /* a(n-2)  */
    seq_t a4 = (seq_t)8;  /* a(n-1)  */
    seq_t an;

    /* Compute terms iteratively up to n. */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t term1 = a4 * 2 + a3 * 12;        /* 2*a(n-1) + 12*a(n-2) */
        seq_t term2 = a2 * 8 + a1 * 16;        /* 8*a(n-3) + 16*a(n-4) */
        an = term1 - term2;                   /* recurrence */

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4; /* a(n) */
}
