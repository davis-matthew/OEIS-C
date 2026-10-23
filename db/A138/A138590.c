/*
 * A138590  a(n) = Fibonacci(9*n).
 * https://oeis.org/A138590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 76*a(n-1) + a(n-2)
 * Initial values: a(0) = 0, a(1) = 34
 */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return 0;
    if (n == 1ULL)
        return 34;

    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 34;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 76 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
