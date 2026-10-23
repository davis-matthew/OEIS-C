/*
 * A049684  a(n) = Fibonacci(2n)^2.
 * https://oeis.org/A049684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 7*a(n-1) - a(n-2) + 2 (for n > 2) with initial values
 *            a(0) = 0, a(1) = 1, a(2) = 9 This recurrence is equivalent to
 *            a(n) = Fibonacci(2n)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 9;

    seq_t a_prev2 = 0;   /* a(0) */
    seq_t a_prev1 = 1;   /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 7 * a_prev1 - a_prev2 + 2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return cur;
}
