/*
 * A206214  Number of (n+1)X8 0..2 arrays with every 2X3 or 3X2 subblock
 *          having exactly two equal edges, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A206214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = init[n-1] for 1 <= n <= 10; a(n) = 258 * a(n-3) for n >
 *            10
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* initial 10 values from the OEIS entry */
    static const seq_t init[10] = {
        (seq_t)34236,
        (seq_t)27656,
        (seq_t)40032,
        (seq_t)274320,
        (seq_t)1435584,
        (seq_t)9438912,
        (seq_t)68074272,
        (seq_t)366583680,
        (seq_t)2427719040,
        (seq_t)17541854784
    };

    /* guard against zero or negative indices */
    if (n == 0) return (seq_t)0;

    /* if n is within the initial table, return directly */
    if (n <= 10) return init[n - 1];

    /* for n > 10, apply the recurrence a(n) = 258 * a(n-3) repeatedly */
    seq_t mul = (seq_t)1;          /* multiplier accumulating 258^(k) */
    idx_t m = n;                   /* working copy of the index */

    while (m > 10) {
        mul *= (seq_t)258;
        m -= 3;
    }

    return init[m - 1] * mul;
}
