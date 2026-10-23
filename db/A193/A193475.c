/*
 * A193475  a(n) = 4*16^n - 2*4^n.
 * https://oeis.org/A193475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*16^n - 2*4^n Recurrence: a(0) = 2, a(1) = 56, a(n) =
 *            20*a(n-1) - 64*a(n-2) (n >= 2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)56;

    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)56;  /* a(1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t next = (seq_t)20 * prev1 - (seq_t)64 * prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
