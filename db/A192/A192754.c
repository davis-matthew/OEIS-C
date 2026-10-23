/*
 * A192754  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*Fibonacci(n) + 3*Lucas(n) - 5 (n ≥ 0)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair_local(idx_t n, seq_t *Fn, seq_t *Fn1)
{
    if (n == (idx_t)0) {            /* base case: F(0)=0, F(1)=1   */
        *Fn  = (seq_t)0;
        *Fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair_local(n >> 1, &a, &b);   /* recursive step on n/2      */

    seq_t two_b = b + b;                     /* 2*F(k+1)                     */
    seq_t c = a * (two_b - a);             /* F(2k)  = F(k)*(2*F(k+1)-F(k)) */
    seq_t d = a * a + b * b;               /* F(2k+1)=F(k)^2+F(k+1)^2       */

    if ((n & (idx_t)1) == (idx_t)0) {      /* n even  →  F(n)=c, F(n+1)=d   */
        *Fn  = c;
        *Fn1 = d;
    } else {                               /* n odd   →  F(n)=d, F(n+1)=c+d */
        *Fn  = d;
        *Fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* compute Fibonacci(n) and Fibonacci(n+1)  */
    seq_t fib_n, fib_n1;
    fib_pair_local(n, &fib_n, &fib_n1);

    /* Lucas number: L(n) = 2*F(n+1) - F(n)     */
    seq_t lucas_n = (fib_n1 << 1) - fib_n;

    /* a(n) = 8*F(n) + 3*L(n) - 5               */
    seq_t result = (fib_n << 3) + (lucas_n * (seq_t)3) - (seq_t)5;
    return result;
}
