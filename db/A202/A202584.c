/*
 * A202584  Number of (n+2) X 3 binary arrays avoiding patterns 001 and 100 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+5*a(n-2)-11*a(n-3)-10*a(n-4)+9*a(n-5)+12*a(n-6)-2*a(n-7)-6*a(n-8)+a(n-10);
 *            Initial values (offset 1): a(1)=96, a(2)=260, a(3)=804,
 *            a(4)=2554, a(5)=8372, a(6)=27649, a(7)=91973, a(8)=306486,
 *            a(9)=1023025, a(10)=3416176
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return 0 for n==0, though offset is 1 */
    if (n == 0)
        return (seq_t)0;

    /* initial 10 terms */
    static const seq_t init[10] = {
        (seq_t)96,          /* a(1) */
        (seq_t)260,         /* a(2) */
        (seq_t)804,         /* a(3) */
        (seq_t)2554,        /* a(4) */
        (seq_t)8372,        /* a(5) */
        (seq_t)27649,       /* a(6) */
        (seq_t)91973,       /* a(7) */
        (seq_t)306486,      /* a(8) */
        (seq_t)1023025,     /* a(9) */
        (seq_t)3416176      /* a(10) */
    };

    if (n <= 10)
        return init[n - 1];

    /* sliding window of the last 10 terms */
    seq_t t1 = init[0], t2 = init[1], t3 = init[2], t4 = init[3], t5 = init[4];
    seq_t t6 = init[5], t7 = init[6], t8 = init[7], t9 = init[8], t10 = init[9];

    for (idx_t i = 11; i <= n; ++i) {
        seq_t next = 3 * t10 + 5 * t9
                    - 11 * t8 - 10 * t7
                    + 9 * t6 + 12 * t5
                    - 2 * t4 - 6 * t3
                    + t1;
        /* rotate window */
        t1 = t2; t2 = t3; t3 = t4; t4 = t5; t5 = t6;
        t6 = t7; t7 = t8; t8 = t9; t9 = t10; t10 = next;
    }

    return t10;
}
