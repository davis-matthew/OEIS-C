/*
 * A124297  a(n) = 5*F(n)^2 + 5*F(n) + 1, where F(n) = Fibonacci(n).
 * https://oeis.org/A124297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*F(n)^2 + 5*F(n) + 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
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
    fib_pair(n >> 1, &a, &b);
    seq_t c = a;
    seq_t d = b;
    /* e = c * (2*d - c) */
    seq_t two_d = d + d;
    seq_t e = c * (two_d - c);
    /* f = c*c + d*d */
    seq_t f = c * c + d * d;
    if ((n & 1) == 0) {
        *fn  = e;
        *fn1 = f;
    } else {
        *fn  = f;
        *fn1 = e + f;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);            /* Fibonacci F(n) */
    seq_t res = fn * fn;              /* F(n)^2 */
    res *= (seq_t)5;                  /* 5*F(n)^2 */
    res += fn * (seq_t)5;             /* +5*F(n) */
    res += (seq_t)1;                  /* +1 */
    return res;
}
