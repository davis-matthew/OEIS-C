/*
 * A184368  1/3 the number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock
 *          having at least two equal elements connected horizontally or
 *          vertically.
 * https://oeis.org/A184368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 9*a(n-2), a(1) = 21, a(2) = 153
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)21;
    }
    if (n == 2) {
        return (seq_t)153;
    }

    seq_t a_prev2 = (seq_t)21;  /* a(1) */
    seq_t a_prev1 = (seq_t)153; /* a(2) */
    seq_t a_curr  = (seq_t)0;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        a_curr = (seq_t)6 * a_prev1 + (seq_t)9 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
