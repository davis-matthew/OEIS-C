/*
 * A234825  Number of (n+1) X (1+1) 0..2 arrays with each 2 X 2 subblock
 *          having the number of clockwise edge increases less than or equal
 *          to the number of counterclockwise edge increases.
 * https://oeis.org/A234825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 12*a(n-2) + 3*a(n-3), a(1)=69, a(2)=516,
 *            a(3)=3843
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)69;
    if (n == 2) return (seq_t)516;
    if (n == 3) return (seq_t)3843;
    seq_t a_prev2 = (seq_t)69;   /* a(n-3) */
    seq_t a_prev1 = (seq_t)516;  /* a(n-2) */
    seq_t a_cur   = (seq_t)3843; /* a(n-1) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 9 * a_cur - 12 * a_prev1 + 3 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
        a_cur   = next;
    }
    return a_cur;
}
