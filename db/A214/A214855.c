/*
 * A214855  Fibonacci numbers divisible by 10.
 * https://oeis.org/A214855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fib(15*(n-1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* compute F(k), F(k+1) for k = n/2 */

    seq_t two_b = b * (seq_t)2;         /* 2*F(k+1)            */
    seq_t c = a * (two_b - a);          /* F(2k)  = F(k)*(2*F(k+1)-F(k)) */
    seq_t d = a * a + b * b;            /* F(2k+1)=F(k)^2+F(k+1)^2            */

    if (n & 1) {                       /* odd n: F(n)=F(2k+1)                 */
        *fn  = d;
        *fn1 = c + d;
    } else {                           /* even n: F(n)=F(2k)                  */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    /* Offset starts at 1: a(1)=fib(0)=0 */
    if (n == 0)
        return 0;

    idx_t k = (n - 1) * (idx_t)15;     /* index of the Fibonacci number   */
    seq_t fn, fn1;
    fib_pair(k, &fn, &fn1);            /* compute F(k) and F(k+1)          */
    return fn;                         /* return a(n)                      */
}
