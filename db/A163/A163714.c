/*
 * A163714  Number of n X 2 binary arrays with all 1s connected, a path of 1s
 *          from top row to bottom row, and no 1 having more than two 1s
 *          adjacent.
 * https://oeis.org/A163714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fibonacci(n+2) for n >= 3. Recurrence: a(n) = a(n-1) +
 *            a(n-2) for n >= 5. Initial values: a(1)=3, a(2)=7, a(3)=10,
 *            a(4)=16
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)16;
    seq_t prev2 = (seq_t)10;  /* a(3) */
    seq_t prev1 = (seq_t)16;  /* a(4) */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
