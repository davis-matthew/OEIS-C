/*
 * A020695  Pisot sequence E(2,3).
 * https://oeis.org/A020695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+3); Recurrence: a(n) = a(n-1) + a(n-2), a(0)
 *            = 2, a(1) = 3
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute F_{n+3} using the standard Fibonacci recurrence.
       We start with F0 = 0, F1 = 1 and iterate until the
       desired index is reached. */
    seq_t f0 = 0;      /* F0 */
    seq_t f1 = 1;      /* F1 */

    /* Target index is n + 3. */
    idx_t target = n + 3;

    /* Handle the trivial cases directly. */
    if (target == 0) return f0;
    if (target == 1) return f1;

    /* Iterate from i = 2 up to and including target. */
    for (idx_t i = 2; i <= target; ++i)
    {
        seq_t next = f0 + f1;
        f0 = f1;
        f1 = next;
    }

    /* At the end of the loop f1 holds F_{n+3}. */
    return f1;
}
