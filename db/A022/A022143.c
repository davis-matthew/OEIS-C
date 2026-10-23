/*
 * A022143  Fibonacci sequence beginning 5, 19.
 * https://oeis.org/A022143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 5, a(1) = 19
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)5;
    }
    if (n == 1) {
        return (seq_t)19;
    }
    seq_t prev2 = (seq_t)5;    /* a(0) */
    seq_t prev1 = (seq_t)19;   /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = prev2 + prev1;  /* a(i) = a(i-2) + a(i-1) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
