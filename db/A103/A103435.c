/*
 * A103435  a(n) = 2^n * Fibonacci(n).
 * https://oeis.org/A103435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * Fibonacci(n) Recurrence: a(0) = 0, a(1) = 2, a(n) =
 *            2 * a(n-1) + 4 * a(n-2) for n ≥ 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)2;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = ((seq_t)2 * prev1) + ((seq_t)4 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
