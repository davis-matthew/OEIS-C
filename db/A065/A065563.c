/*
 * A065563  Product of three consecutive Fibonacci numbers.
 * https://oeis.org/A065563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) * Fibonacci(n+1) * Fibonacci(n+2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Fibonacci numbers by fast doubling: fib_pair(n) returns
   fib(n) in *fn and fib(n+1) in *fn1.  Uses 0‑based indexing: fib(0)=0, fib(1)=1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = fib(k), b = fib(k+1) where k = n/2 */

    seq_t two_b   = b + b;             /* 2 * fib(k+1) */
    seq_t c       = a * (two_b - a);  /* fib(2k)   = fib(k) * (2*fib(k+1) - fib(k)) */
    seq_t d       = a * a + b * b;     /* fib(2k+1) = fib(k)^2 + fib(k+1)^2 */

    if ((n & 1) == 0) {               /* n even:  fib(n)=c, fib(n+1)=d */
        *fn  = c;
        *fn1 = d;
    } else {                           /* n odd :  fib(n)=d, fib(n+1)=c + d */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);       /* fn  = fib(n),     fn1 = fib(n+1)   */
    seq_t fn2 = fn + fn1;         /* fib(n+2) = fib(n) + fib(n+1) */
    return fn * fn1 * fn2;        /* product of three consecutive Fibonacci numbers */
}
