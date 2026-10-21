/*
 * A097133  a(n) = 3*Fibonacci(n) + (-1)^n.
 * https://oeis.org/A097133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*Fibonacci(n) + (-1)^n
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 5, ... */

seq_t a(idx_t n)
{
    /* Compute F(n) iteratively (F(0)=0, F(1)=1) */
    seq_t f0 = (seq_t)0;          /* F(i-1)   */
    seq_t f1 = (seq_t)1;          /* F(i)     */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t tmp = f0 + f1;   /* F(i+1)   */
        f0 = f1;               /* shift      */
        f1 = tmp;
    }
    /* After loop: f0 == F(n) */
    seq_t fib = f0;

    /* a(n) = 3*Fib(n) + (-1)^n */
    seq_t res = fib * ((seq_t)3);
    if ((n & (idx_t)1) == 0)   /* n even -> (-1)^n = +1 */
        res = res + ((seq_t)1);
    else                       /* n odd  -> (-1)^n = -1 */
        res = res - ((seq_t)1);

    return res;
}
