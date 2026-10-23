/*
 * A138134  a(n) = Sum_{i=0..n} Fibonacci(5*i).
 * https://oeis.org/A138134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 11*a(n-1) + a(n-2) + 5 (n > 1) Initial values: a(0) = 0,
 *            a(1) = 5 This yields a(n) = Σ_{i=0..n} Fibonacci(5*i)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 5;

    seq_t a_prevprev = 0;   /* a(0) */
    seq_t a_prev = 5;       /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 11 * a_prev + a_prevprev + 5;
        a_prevprev = a_prev;
        a_prev = cur;
    }

    return cur;
}
