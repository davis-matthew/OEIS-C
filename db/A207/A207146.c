/*
 * A207146  Number of (n+1) X 6 0..2 arrays with every 2 X 2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A207146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) for n > 12 Initial values:
 *            a(1)=159, a(2)=110, a(3)=311, a(4)=218, a(5)=622, a(6)=434,
 *            a(7)=1238, a(8)=866, a(9)=2470, a(10)=1730, a(11)=4934,
 *            a(12)=3458
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 50, u64 n <= 114, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first 12 terms directly */
    static const seq_t init[12] = {
        159, 110, 311, 218, 622,
        434, 1238, 866, 2470, 1730,
        4934, 3458
    };
    if (n == 0) {
        /* Undefined for n=0; return 0 to avoid UB */
        return (seq_t)0;
    }
    if (n <= 12) {
        return init[n - 1];
    }

    /* Recurrence: a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) */
    seq_t prev1 = init[11];  /* a(12) */
    seq_t prev2 = init[10];  /* a(11) */
    seq_t prev3 = init[9];   /* a(10) */

    for (idx_t k = 13; k <= n; ++k) {
        seq_t cur = prev1 + (seq_t)2 * prev2 - (seq_t)2 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
