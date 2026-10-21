/*
 * A087404  a(n) = 4*a(n-1) + 5*a(n-2) for n > 1, with a(0) = 2 and a(1) = 4.
 * https://oeis.org/A087404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + 5*a(n-2) (n > 1), with a(0) = 2, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;

    /* Use an iterative recurrence, which works for all supported types
       (plain integral types as well as arbitrary‑precision integer classes),
       because the operations are only additions and multiplications by
       small constants. */
    seq_t prev2 = (seq_t)2;   /* a(n-2) */
    seq_t prev1 = (seq_t)4;   /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)4 * prev1 + (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
