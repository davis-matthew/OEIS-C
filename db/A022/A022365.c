/*
 * A022365  Fibonacci sequence beginning 0, 31.
 * https://oeis.org/A022365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31 * Fibonacci(n) where Fibonacci(0)=0, Fibonacci(1)=1
 *            Recurrence: a(0)=0, a(1)=31, a(n)=a(n-1)+a(n-2) for n≥2
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)31;

    seq_t prev = (seq_t)31;   /* a(n-1) */
    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t curr;
    idx_t i;

    for (i = 2; i <= n; ++i) {
        curr = prev + prev2;  /* a(n) */
        prev2 = prev;         /* shift a(n-2) <- a(n-1) */
        prev = curr;          /* shift a(n-1) <- a(n)   */
    }
    return prev;              /* a(n) */
}
