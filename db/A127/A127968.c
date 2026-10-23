/*
 * A127968  a(n) = F(n+1) + (1-(-1)^n)/2, where F() = Fibonacci numbers
 *          A000045.
 * https://oeis.org/A127968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127968
   Formula: a(n) = F(n+1) + (1-(-1)^n)/2
   where F(k) is the k-th Fibonacci number (F(0)=0, F(1)=1).
*/

static seq_t fib_iter(idx_t n)
{
    /* iterative Fibonacci: return F(n) */
    seq_t a = 0, b = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* F(n+1) + 1 if n is odd, otherwise +0 */
    seq_t fn1 = fib_iter(n + 1);
    return fn1 + (n & 1);
}
