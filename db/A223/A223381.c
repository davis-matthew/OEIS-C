/*
 * A223381  Number of n X 2 0..2 arrays with all horizontally or vertically
 *          connected equal values in a straight line, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A223381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 10*a(n-2) for n>3; a(1)=2, a(2)=9, a(3)=57
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)57;

    seq_t prev2 = (seq_t)9;      /* a(2) */
    seq_t prev1 = (seq_t)57;     /* a(3) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = prev1 * (seq_t)8 - prev2 * (seq_t)10;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
