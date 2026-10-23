/*
 * A022346  Fibonacci sequence beginning 0, 12.
 * https://oeis.org/A022346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*F(n) (F = Fibonacci), with recurrence a(0) = 0, a(1) =
 *            12, a(2) = 12, a(n) = a(n-1) + a(n-2) for n ≥ 3
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) {
        return (seq_t)0;
    }
    /* a(1) = 12, a(2) = 12 */
    if (n == 1U || n == 2U) {
        return (seq_t)12;
    }

    seq_t a0 = (seq_t)0;    /* a(0) */
    seq_t a1 = (seq_t)12;   /* a(1) */
    for (idx_t i = 2U; i <= n; ++i) {
        seq_t a_next = a1 + a0;  /* a(i) = a(i-1) + a(i-2) */
        a0 = a1;
        a1 = a_next;
    }
    return a1;
}
