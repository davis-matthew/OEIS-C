/*
 * A022393  Fibonacci sequence beginning 1, 23.
 * https://oeis.org/A022393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) Initial values: a(0) = 1, a(1) = 23
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)23;
    }
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)23;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
