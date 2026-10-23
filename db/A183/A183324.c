/*
 * A183324  Number of nX3 binary arrays with each 1 adjacent to exactly two
 *          other 1s.
 * https://oeis.org/A183324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 70, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=2*a(n-1)-a(n-2)+2*a(n-3)-a(n-4)   for n≥5
   Initial values: a(1)=1, a(2)=3, a(3)=6, a(4)=10
   Computes the number of n×3 binary arrays with each 1 adjacent to exactly two other 1s.
   Iterative implementation avoids dynamic memory and works for any
   integer (fixed‑width or arbitrary‑precision) type seq_t. */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)10;

    seq_t a1 = (seq_t)1, a2 = (seq_t)3, a3 = (seq_t)6, a4 = (seq_t)10, a5;

    for (idx_t i = 5; i <= n; ++i) {
        a5 = (seq_t)2 * a4 - a3 + (seq_t)2 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }

    return a4;
}
