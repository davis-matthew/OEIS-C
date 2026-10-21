/*
 * A233235  Number of nX4 0..5 arrays with no element x(i,j) adjacent to value
 *          5-x(i,j) horizontally, diagonally or antidiagonally, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order.
 * https://oeis.org/A233235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 311*a(n-1) - 8722*a(n-2) + 10022*a(n-3) - 1645*a(n-4) +
 *            35*a(n-5) (recurrence, initial values: a(1)=23, a(2)=4504,
 *            a(3)=1212549, a(4)=338046654, a(5)=94601758339)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset undefined; safe return */
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)4504;
    if (n == 3) return (seq_t)1212549;
    if (n == 4) return (seq_t)338046654;
    if (n == 5) return (seq_t)94601758339;

    /* Store the last five terms, with f[0] = a(k) ,
       f[1] = a(k-1), ..., f[4] = a(k-4) */
    seq_t f[5];
    f[0] = (seq_t)94601758339;   /* a(5) */
    f[1] = (seq_t)338046654;     /* a(4) */
    f[2] = (seq_t)1212549;       /* a(3) */
    f[3] = (seq_t)4504;          /* a(2) */
    f[4] = (seq_t)23;            /* a(1) */

    idx_t k;
    for (k = 6; k <= n; ++k)
    {
        seq_t A = (seq_t)311 * f[0]           /* 311*a(k-1) */
                + (seq_t)10022 * f[2]           /* +10022*a(k-3) */
                + (seq_t)35  * f[4];           /* +35*a(k-5) */

        seq_t B = (seq_t)8722 * f[1]           /* 8722*a(k-2) */
                + (seq_t)1645 * f[3];           /* +1645*a(k-4) */

        seq_t next = A - B;                    /* a(k) = A - B */

        /* shift the window */
        f[4] = f[3];
        f[3] = f[2];
        f[2] = f[1];
        f[1] = f[0];
        f[0] = next;
    }
    return f[0];
}
