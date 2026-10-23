/*
 * A116782  Number of permutations of length n which avoid the patterns 3421,
 *          4123, 4231; or avoid the patterns 1342, 3142, 4213.
 * https://oeis.org/A116782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=6, a(4)=21; For n>=5:
 *            a(n)=6*a(n-1)-11*a(n-2)+9*a(n-3)-2*a(n-4)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small initial values */
    static const seq_t init[4] = { (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)21 };

    if (n == 0)                 /* not used – sequence starts at 1 */
        return (seq_t)0;
    if (n <= 4)                 /* return one of the initial values */
        return init[n-1];

    /* Working variables: a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1) */
    seq_t a1 = init[0];
    seq_t a2 = init[1];
    seq_t a3 = init[2];
    seq_t a4 = init[3];
    seq_t cur;                  /* holds a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        /* recurrence: a(n)=6*a(n-1)-11*a(n-2)+9*a(n-3)-2*a(n-4) */
        cur = a4 * (seq_t)6;            /* 6*a(n-1) */
        cur -= a3 * (seq_t)11;          /* -11*a(n-2) */
        cur += a2 * (seq_t)9;           /* +9*a(n-3) */
        cur -= a1 * (seq_t)2;           /* -2*a(n-4) */

        /* shift variables for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }

    return a4;          /* a(n) */
}
