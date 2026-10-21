/*
 * A205220  Number of (n+1) X 3 0..1 arrays with the number of equal 2 X 2
 *          subblock diagonal pairs and equal antidiagonal pairs differing
 *          from each horizontal or vertical neighbor, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A205220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2); a(1) = 20, a(2) = 52
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    const seq_t A1 = (seq_t)20;
    if (n == 1)
        return A1;
    const seq_t A2 = (seq_t)52;
    if (n == 2)
        return A2;

    seq_t prev = A1; /* a(n-2)   */
    seq_t curr = A2; /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = curr + (prev << 2); /* curr + 4*prev */
        prev = curr;
        curr = next;
    }
    return curr;
}
