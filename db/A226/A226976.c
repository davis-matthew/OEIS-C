/*
 * A226976  Fibonacci(n)^3 + Fibonacci(n+2)^3.
 * https://oeis.org/A226976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n)^3 + Fibonacci(n+2)^3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the pair (F(n), F(n+1)) using the fast‑doubling method. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);            /* a = F(k),  b = F(k+1) */
    seq_t two_b = b + b;                 /* 2*b */
    seq_t c = a * (two_b - a);           /* F(2k)   */
    seq_t d = a * a + b * b;             /* F(2k+1) */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);          /* fn = F(n),   fn1 = F(n+1) */
    seq_t fn2 = fn + fn1;             /* F(n+2) = F(n) + F(n+1) */

    seq_t term1 = fn * fn * fn;       /* F(n)^3  */
    seq_t term2 = fn2 * fn2 * fn2;    /* F(n+2)^3 */

    return term1 + term2;
}
