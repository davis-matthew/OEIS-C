/*
 * A104220  a(n) = 1 + Fibonacci(n) - (Fibonacci(n) mod 2).
 * https://oeis.org/A104220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Fibonacci(n) - (Fibonacci(n) mod 2)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) equals Fibonacci(n) if Fibonacci(n) is odd, otherwise Fibonacci(n)+1. */

static void fib_pair(idx_t n, seq_t *f_n, seq_t *f_n1)
{
    if (n == 0) {
        *f_n = 0;
        *f_n1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);
    seq_t two_b = b << 1;
    seq_t t = two_b - a;
    seq_t c = a * t;          /* F(2k)   */
    seq_t aa = a * a;
    seq_t bb = b * b;
    seq_t d = aa + bb;        /* F(2k+1) */
    if ((n & 1) == 0) {
        *f_n  = c;
        *f_n1 = d;
    } else {
        *f_n  = d;
        *f_n1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t f, f_next;
    fib_pair(n, &f, &f_next);
    if ((f & 1) == 0)
        f = f + 1;
    return f;
}
