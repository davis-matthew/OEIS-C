/*
 * A107300  a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3) with a(0)=3, a(1)=2, a(3)=8.
 * https://oeis.org/A107300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3), a(0)=3, a(1)=2, a(2)=8
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This iterative implementation uses only n, the recurrence, and a fixed
   small amount of state.  It works for any integer type seq_t (built‑in or
   arbitrary‑precision class).  The algorithm is O(n) time and O(1) space. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    seq_t a0 = (seq_t)3;   /* a(n-3) */
    seq_t a1 = (seq_t)2;   /* a(n-2) */
    seq_t a2 = (seq_t)8;   /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)2 * a2 + (seq_t)2 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
