/*
 * A022093  Fibonacci sequence beginning 0, 10.
 * https://oeis.org/A022093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10 * Fib(n) (Fibonacci sequence with F(0)=0, F(1)=1)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly for clarity */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)10;

    /* Iteratively compute Fib(n) */
    seq_t prev = (seq_t)0;      /* F(0) */
    seq_t curr = (seq_t)1;      /* F(1) */

    /* Compute up to F(n) */ 
    for (idx_t i = 1; i < n; ++i) {
        seq_t tmp = prev + curr;   /* F(i+1) = F(i-1) + F(i) */
        prev = curr;
        curr = tmp;
    }

    /* Multiply by 10 to obtain a(n) */
    return curr * (seq_t)10;
}
