/*
 * A073372  Second convolution of A001045(n+1) (generalized (1,2)-Fibonacci),
 *          n>=0, with itself.
 * https://oeis.org/A073372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+3*a(n-2)-11*a(n-3)-6*a(n-4)+12*a(n-5)+8*a(n-6)
 *            with a(0)=1, a(1)=3, a(2)=12, a(3)=34, a(4)=99, a(5)=261
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Base values for n = 0..5 */
    static const seq_t base[6] = { (seq_t)1, (seq_t)3, (seq_t)12,
                                   (seq_t)34, (seq_t)99, (seq_t)261 };
    if (n < 6) return base[n];

    /* Hold the last six terms: a[i-6],...,a[i-1]  */
    seq_t a6[6];
    a6[0] = base[0]; /* a[i-6] */
    a6[1] = base[1]; /* a[i-5] */
    a6[2] = base[2]; /* a[i-4] */
    a6[3] = base[3]; /* a[i-3] */
    a6[4] = base[4]; /* a[i-2] */
    a6[5] = base[5]; /* a[i-1] */

    /* Compute terms up to index n */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = (seq_t)3 * a6[5]            /* 3*a(n-1) */
                   + (seq_t)3 * a6[4]            /* 3*a(n-2) */
                   - (seq_t)11 * a6[3]           /* -11*a(n-3) */
                   - (seq_t)6  * a6[2]           /* -6*a(n-4)  */
                   + (seq_t)12 * a6[1]           /* +12*a(n-5) */
                   + (seq_t)8  * a6[0];          /* +8*a(n-6)  */

        /* Shift window: drop a[i-6], slide others up */
        a6[0] = a6[1];
        a6[1] = a6[2];
        a6[2] = a6[3];
        a6[3] = a6[4];
        a6[4] = a6[5];
        a6[5] = next;
    }
    return a6[5]; /* a[n] */
}
