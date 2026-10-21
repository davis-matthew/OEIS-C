/*
 * A210773  Number of partitions of 2^n into powers of 2 less than or equal to
 *          16.
 * https://oeis.org/A210773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) - 310*a(n-2) + 1240*a(n-3) - 1984*a(n-4) +
 *            1024*a(n-5) (n ≥ 9) with initial values a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=10, a(4)=36, a(5)=201, a(6)=1625, a(7)=17361, a(8)=222241
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        (seq_t)1,      /* a0 */
        (seq_t)2,      /* a1 */
        (seq_t)4,      /* a2 */
        (seq_t)10,     /* a3 */
        (seq_t)36,     /* a4 */
        (seq_t)201,    /* a5 */
        (seq_t)1625,   /* a6 */
        (seq_t)17361,  /* a7 */
        (seq_t)222241  /* a8 */
    };

    if (n < 9) {
        return init[n];
    }

    /* Keep the last five terms: a[n-5] … a[n-1] */
    seq_t a_prev[5];
    a_prev[0] = init[4]; /* a4 */
    a_prev[1] = init[5]; /* a5 */
    a_prev[2] = init[6]; /* a6 */
    a_prev[3] = init[7]; /* a7 */
    a_prev[4] = init[8]; /* a8 */

    for (idx_t i = 9; i <= n; ++i) {
        seq_t pos = (seq_t)31 * a_prev[4]          /* 31*a(n-1) */
                  + (seq_t)1240 * a_prev[2]        /* 1240*a(n-3) */
                  + (seq_t)1024 * a_prev[0];       /* 1024*a(n-5) */

        seq_t neg = (seq_t)310 * a_prev[3]         /* 310*a(n-2) */
                  + (seq_t)1984 * a_prev[1];      /* 1984*a(n-4) */

        seq_t next = pos - neg;                    /* positive result */

        /* shift for next iteration */
        a_prev[0] = a_prev[1];
        a_prev[1] = a_prev[2];
        a_prev[2] = a_prev[3];
        a_prev[3] = a_prev[4];
        a_prev[4] = next;
    }

    return a_prev[4];
}
