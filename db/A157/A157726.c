/*
 * A157726  a(n) = Fibonacci(n) + 3.
 * https://oeis.org/A157726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) + 3 a(0) = 3, a(1) = 4 Recurrence: a(n) =
 *            a(n-1) + a(n-2) - 3 (for n ≥ 2)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)4;

    /* Iterative computation using the recurrence relation. */
    seq_t prev = (seq_t)3;   /* a(0) */
    seq_t curr = (seq_t)4;   /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = curr + prev - (seq_t)3;
        prev = curr;
        curr = next;
    }

    return curr;
}
