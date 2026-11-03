/*
 * A139764  Smallest term in Zeckendorf representation of n.
 * https://oeis.org/A139764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n if n is a Fibonacci number (1,2,3,5,…), * otherwise
 *            a(n) = a( n - (largest Fibonacci number < n) )
 * Range:     u8 n <= 376, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 105 terms through n = 9572 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A helper that returns the largest Fibonacci number ≤ x.
 * Fibonacci numbers used: 1, 2, 3, 5, 8, …  (F(1)=1, F(2)=2). */
static seq_t largest_fib_le (seq_t x)
{
    if (x <= (seq_t)1) return (seq_t)1;   /* covers x==0 and x==1 */
    seq_t a = (seq_t)1;
    seq_t b = (seq_t)2;
    seq_t c;
    while (b <= x) {
        c = a + b;  /* compute next Fibonacci number   */
        a = b;
        b = c;
    }
    return a;  /* a is the largest Fibonacci ≤ x   */
}

seq_t a (idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not defined for 0, but safe */
    seq_t x = (seq_t)n;
    while (1) {
        seq_t f = largest_fib_le(x);
        if (f == x) return x;             /* x itself is Fibonacci */
        x = x - f;                        /* otherwise subtract the largest one */
    }
}
