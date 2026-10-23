/*
 * A156084  Sum of the squares of the first n Fibonacci numbers with index
 *          divisible by 3.
 * https://oeis.org/A156084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156084:  a(n) = sum_{k=1..n} F(3k)^2
   Equivalent formulas:
       a(n) = L(6n+3)/20 - (-1)^n/5
       Recurrence: a(n) - 17 a(n-1) - 17 a(n-2) + a(n-3) = 0
       Initial values: a(0)=0, a(1)=4, a(2)=68
*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 4;
    if (n == 2) return 68;

    seq_t a0 = 0;
    seq_t a1 = 4;
    seq_t a2 = 68;
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = 17 * a2 + 17 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
