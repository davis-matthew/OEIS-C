/*
 * A228994  Total sum of the 4th powers of lengths of ascending runs in all
 *          permutations of [n].
 * https://oeis.org/A228994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*a(n-1) - n*(n-1)*a(n-2) + (12*n*n - 24*n + 14)
 *            Initial values: a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t a0 = (seq_t)0;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t an = a1;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = 2*i*a(i-1) */
        seq_t term1 = (seq_t)2 * (seq_t)i * a1;

        /* term2 = i*(i-1)*a(i-2) */
        seq_t term2 = (seq_t)i * (seq_t)(i - 1) * a0;

        /* cubic = 12*i*i - 24*i + 14 */
        seq_t cubic = (seq_t)12 * (seq_t)i * (seq_t)i
                    - (seq_t)24 * (seq_t)i
                    + (seq_t)14;

        an = term1 - term2 + cubic;

        /* shift for next iteration */
        a0 = a1;
        a1 = an;
    }

    return an;
}
