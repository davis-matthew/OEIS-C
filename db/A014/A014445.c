/*
 * A014445  Even Fibonacci numbers; or, Fibonacci(3*n).
 * https://oeis.org/A014445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 2, a(n) = 4*a(n-1) + a(n-2) for n ≥ 2. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;

    seq_t prev2 = 0;          /* a(n-2) */
    seq_t prev1 = 2;          /* a(n-1) */
    seq_t curr  = 0;          /* a(n)   */

    for (idx_t i = 2; i <= n; ++i) {
        curr = 4 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
