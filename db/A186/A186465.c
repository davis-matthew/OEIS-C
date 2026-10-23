/*
 * A186465  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + 4*a(n-4) - 2*a(n-5) for n
 *            > 11, with initial conditions a(1) = 83, a(2) = 129, a(3) = 243,
 *            a(4) = 562, a(5) = 903, a(6) = 1754, a(7) = 2937, a(8) = 5790,
 *            a(9) = 9993, a(10) = 19236, a(11) = 33546
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 30, u64 n <= 67, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[11] = {
        (seq_t)83,  (seq_t)129, (seq_t)243, (seq_t)562, (seq_t)903,
        (seq_t)1754, (seq_t)2937, (seq_t)5790, (seq_t)9993, (seq_t)19236,
        (seq_t)33546
    };

    if (n <= 11ULL)
        return init[n - 1];  /* a(1) -> init[0], etc. */

    /* Sliding window: a1 = a(n-5), ..., a5 = a(n-1) */
    seq_t a1 = init[6];  /* a(7) */
    seq_t a2 = init[7];  /* a(8) */
    seq_t a3 = init[8];  /* a(9) */
    seq_t a4 = init[9];  /* a(10) */
    seq_t a5 = init[10]; /* a(11) */
    seq_t an = 0;        /* placeholder for a(n) */

    for (idx_t i = 12ULL; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        seq_t sum_pos = a5 * 2 + a4 + a2 * 4;   /* 2*a5 + a4 + 4*a2 */
        seq_t sum_neg = a3 * 4 + a1 * 2;        /* 4*a3 + 2*a1   */
        an = sum_pos - sum_neg;                 /* a(i) = sum_pos - sum_neg */

        /* Shift the sliding window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return an;
}
