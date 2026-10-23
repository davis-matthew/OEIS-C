/*
 * A106729  Sum of two consecutive squares of Lucas numbers (A001254).
 * https://oeis.org/A106729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106729: a(n) = Lucas(n)^2 + Lucas(n+1)^2
   Recurrence: a(0)=5, a(1)=10,
                a(n)=3*a(n-1)-a(n-2)  for n>=2. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)10;

    seq_t prev2 = (seq_t)5;   /* a(0) */
    seq_t prev1 = (seq_t)10;  /* a(1) */
    seq_t curr  = prev1;      /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)3 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
