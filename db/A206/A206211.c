/*
 * A206211  Number of (n+1)X5 0..2 arrays with every 2X3 or 3X2 subblock
 *          having exactly two equal edges, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A206211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1..7) = 594, 992, 2224, 8136, 21984, 74080, 274320. For n > 7:
 *            a(n) = 34 * a(n-3). The function recomputes each value
 *            independently, so it works for arbitrary-precision seq_t and for
 *            fixed-width types
 * Range:     u8 none, u16 n <= 5, u32 n <= 15, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        /* n < 1 is out of the defined offset; return zero */
        return (seq_t)0;
    }

    /* Initial values for 1 <= n <= 7 */
    static const seq_t init[7] = {
        (seq_t)594,
        (seq_t)992,
        (seq_t)2224,
        (seq_t)8136,
        (seq_t)21984,
        (seq_t)74080,
        (seq_t)274320
    };

    if (n <= 7) {
        return init[n - 1];
    }

    /* Sliding window to apply the recurrence:
       a_minus3 = a(i-3), a_minus2 = a(i-2), a_minus1 = a(i-1)  */
    seq_t a_minus3 = init[4]; /* a(5) */
    seq_t a_minus2 = init[5]; /* a(6) */
    seq_t a_minus1 = init[6]; /* a(7) */
    seq_t cur = a_minus1;     /* placeholder for the current value */

    for (idx_t i = 8; i <= n; ++i) {
        cur = a_minus3 * (seq_t)34;  /* a(i) = 34 * a(i-3) */
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = cur;
    }

    return a_minus1; /* This holds a(n) after the loop */
}
