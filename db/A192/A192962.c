/*
 * A192962  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*Fibonacci(n+1) + 4*Fibonacci(n) - 2*(n+2)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle undefined or small indices */
    if (n == 0) return (seq_t)0;
    if (n <= 4) {
        static const seq_t init[] = { (seq_t)1, (seq_t)2, (seq_t)7, (seq_t)15 };
        return init[n - 1];
    }

    /* Compute Fibonacci numbers up to F(n+1) using addition only */
    seq_t f0 = (seq_t)0; /* F(0) */
    seq_t f1 = (seq_t)1; /* F(1) */
    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t f2 = f0 + f1; /* F(i) */
        f0 = f1;
        f1 = f2;
    }
    seq_t fib_n  = f0; /* F(n)   */
    seq_t fib_np1 = f1;/* F(n+1) */

    /* 3 * F(n+1) */
    seq_t t   = fib_np1 + fib_np1;    /* 2 * F(n+1) */
    seq_t three_fib_np1 = t + fib_np1;/* 3 * F(n+1) */

    /* 4 * F(n) */
    seq_t t2  = fib_n + fib_n;       /* 2 * F(n)   */
    seq_t four_fib_n = t2 + t2;      /* 4 * F(n)   */

    seq_t result = three_fib_np1 + four_fib_n;

    /* Subtract 2*(n+2) */
    seq_t sub = (seq_t)(n + 2);
    sub = sub + sub;                 /* 2*(n+2)    */
    result = result - sub;

    return result;
}
