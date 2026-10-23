/*
 * A015443  Generalized Fibonacci numbers: a(n) = a(n-1) + 8*a(n-2).
 * https://oeis.org/A015443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 8*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0 || n == 1)
        return 1;

    /* Iterate the recurrence up to the desired index. */
    seq_t prev_prev = 1; /* a(n-2)  */
    seq_t prev      = 1; /* a(n-1)  */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev + ((seq_t)8 * prev_prev);
        prev_prev = prev;
        prev = curr;
    }

    return prev;
}
