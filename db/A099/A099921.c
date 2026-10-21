/*
 * A099921  a(n) = 5*Fibonacci(n)^2.
 * https://oeis.org/A099921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A099921: a(n) = 5 * Fibonacci(n)^2
   Recurrence: a(1)=5, a(2)=5, a(3)=20, a(n)=2*a(n-1)+2*a(n-2)-a(n-3) for n>3 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; n==0 is unused but harmless */
    if (n == 1) return 5;
    if (n == 2) return 5;
    if (n == 3) return 20;

    seq_t prev3 = 5;   /* a(n-3) */
    seq_t prev2 = 5;   /* a(n-2) */
    seq_t prev1 = 20;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = prev1 * 2 + prev2 * 2 - prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
