/*
 * A072134  Second generation sequence in which each number is skipped that
 *          can be written as sum of distinct previous entries. To make the
 *          first generation we start with all natural numbers: this gives the
 *          powers of 2 (A000079). For the second generation we start with the
 *          natural numbers from which are remo...
 * https://oeis.org/A072134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-1) + 3*a(n-2) + 3*a(n-3) for n>5 Initial values:
 *            a(0)=3, a(1)=5, a(2)=6, a(3)=7, a(4)=17, a(5)=19
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constant initial segment */
    static const seq_t init[6] = {3, 5, 6, 7, 17, 19};
    if (n < 6) return init[n];

    /* Working variables: a = a(i-3), b = a(i-2), c = a(i-1) */
    seq_t a = init[3];     /* a(3)  */
    seq_t b = init[4];     /* a(4)  */
    seq_t c = init[5];     /* a(5)  */
    seq_t val;

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = -a(i-1) + 3*a(i-2) + 3*a(i-3) */
        val = 3 * b + 3 * a - c;
        a = b;         /* shift window */
        b = c;
        c = val;
    }
    return c;
}
