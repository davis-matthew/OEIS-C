/*
 * A080097  a(n) = Fibonacci(n+2)^2 - 1.
 * https://oeis.org/A080097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n+2)^2 - 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 912 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_fast(idx_t n, seq_t *fn, seq_t *fn1)
{
    /* Fast doubling algorithm.
       Returns F(n) in *fn and F(n+1) in *fn1. */
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_fast(n >> 1, &a, &b);          /* a = F(k), b = F(k+1)   k = n/2 */
    seq_t two_b = b + b;                /* 2*F(k+1) */
    seq_t c = a * (two_b - a);         /* F(2k)   */
    seq_t d = a * a + b * b;           /* F(2k+1) */
    if ((n & 1) == 0) {
        *fn = c;
        *fn1 = d;
    } else {
        *fn = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    idx_t target = n + 2;              /* we need Fibonacci(n+2)   */
    seq_t fn, fn_next;
    fib_fast(target, &fn, &fn_next);   /* fn = F(n+2)            */
    seq_t res = fn * fn;               /* square the Fibonacci    */
    res -= 1;                          /* subtract one            */
    return res;
}
