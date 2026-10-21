/*
 * A211619  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and 2w+x+y>2.
 * https://oeis.org/A211619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4) - a(n-5)
 *            Initial values: a(0)=0, a(1)=1, a(2)=18, a(3)=73, a(4)=192,
 *            a(5)=395, a(6)=710
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms a(0)…a(6) */
    static const seq_t init[7] = { (seq_t)0, (seq_t)1, (seq_t)18, (seq_t)73,
                                   (seq_t)192, (seq_t)395, (seq_t)710 };
    if (n <= 6)
        return init[n];

    /* Sliding window for the recurrence */
    seq_t t0 = init[2]; /* a(n-5) */
    seq_t t1 = init[3]; /* a(n-4) */
    seq_t t2 = init[4]; /* a(n-3) */
    seq_t t3 = init[5]; /* a(n-2) */
    seq_t t4 = init[6]; /* a(n-1) */
    seq_t ai = 0;       /* will hold a(n) */

    for (idx_t i = 7; i <= n; ++i) {
        ai = (seq_t)3 * t4
          - (seq_t)2 * t3
          - (seq_t)2 * t2
          + (seq_t)3 * t1
          - t0;

        /* shift window */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = ai;
    }
    return ai;
}
