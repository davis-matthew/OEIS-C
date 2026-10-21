/*
 * A085504  Horadam sequence (0,1,9,3).
 * https://oeis.org/A085504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=18, a(3)=81; a(n)=3*a(n-1)+9*a(n-2) for n>=4 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)18;
    if (n == 3) return (seq_t)81;

    seq_t prev2 = (seq_t)18;   /* a(2) */
    seq_t prev1 = (seq_t)81;   /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)3 * prev1 + (seq_t)9 * prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
