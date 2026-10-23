/*
 * A022368  Fibonacci sequence beginning 2, 12.
 * https://oeis.org/A022368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(Fib(n+2) + 4*Fib(n))
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
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
    fib_pair(n >> 1, &a, &b);                       /* a = F(k),  b = F(k+1)   */
    seq_t two = 2;
    seq_t c = a * (b * two - a);                   /* F(2k)   */
    seq_t d = a * a + b * b;                       /* F(2k+1) */
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
    seq_t fn, fn1;          /* Fn = Fib(n),  Fn1 = Fib(n+1) */
    fib_pair(n, &fn, &fn1);
    seq_t fn2 = fn + fn1;   /* Fib(n+2) = Fib(n) + Fib(n+1) */
    seq_t val = fn * 4 + fn2;   /* 4*Fib(n) + Fib(n+2) */
    val *= 2;                    /* multiply by 2 */
    return val;
}
