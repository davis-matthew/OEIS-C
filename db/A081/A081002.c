/*
 * A081002  a(n) = Fibonacci(4n) + 1, or Fibonacci(2n-1)*Lucas(2n+1).
 * https://oeis.org/A081002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4n) + 1, where Fibonacci(0)=0, Fibonacci(1)=1.
 *            The computation uses the fast‑doubling algorithm, which runs in
 *            O(log n) time and requires only arithmetic on the seq_t type
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);        /* a = F(k), b = F(k+1)  (k = n/2) */

    seq_t two_b = b + b;              /* 2·F(k+1) */
    seq_t c = a * (two_b - a);        /* F(2k)   */
    seq_t d = a * a + b * b;          /* F(2k+1) */

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
    idx_t four_n = n * (idx_t)4;      /* 4 n – exact for all test ranges */
    seq_t f4n, f4n1;
    fib_pair(four_n, &f4n, &f4n1);    /* compute F(4n) and F(4n+1) */
    return f4n + (seq_t)1;            /* a(n) = F(4n) + 1 */
}
