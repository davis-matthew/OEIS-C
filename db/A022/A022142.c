/*
 * A022142  Fibonacci sequence beginning 5, 18.
 * https://oeis.org/A022142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 5, a(1) = 18
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial terms directly. */
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)18;

    /* Iteratively compute the recurrence using two variables. */
    seq_t prev = (seq_t)5;   /* a(n-2) */
    seq_t curr = (seq_t)18;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = prev + curr; /* a(n) = a(n-1)+a(n-2) */
        prev = curr;
        curr = next;
    }
    return curr;
}
