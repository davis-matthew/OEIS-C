/*
 * A125817  a(n) = ((1 + 3*sqrt(2))^n - (1 - 3*sqrt(2))^n)/(2*sqrt(2)).
 * https://oeis.org/A125817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=3, a(n)=2*a(n-1)+17*a(n-2) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)0;  /* a(n-2)  */
    seq_t prev1 = (seq_t)3;  /* a(n-1)  */
    seq_t curr  = 0;         /* a(n)    */

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)2 * prev1 + (seq_t)17 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
