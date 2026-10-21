/*
 * A081014  a(n) = Lucas(4*n+1) + 1, or Lucas(2*n)*Lucas(2*n+1).
 * https://oeis.org/A081014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4*n+1) + 1 Lucas numbers satisfy L(0)=2, L(1)=1,
 *            L(n)=L(n-1)+L(n-2). We compute Lucas via Fibonacci
 *            fast‑doubling: L(n) = 2*F(n+1) - F(n) (F(0)=0,F(1)=1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
/* Computes Fibonacci numbers F(n) and F(n+1) */
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);

    seq_t two_b = b + b;            /* 2*F(k+1) */
    c = a * (two_b - a);            /* F(2k)   */
    d = a * a + b * b;              /* F(2k+1) */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
/* Returns the A081014 sequence value at index n */
{
    idx_t m = 4 * n + 1;            /* n' = 4*n + 1 */
    seq_t fn, fn1;
    fib_pair(m, &fn, &fn1);         /* fn = F(m), fn1 = F(m+1) */

    seq_t L = fn1 + fn1;            /* 2*F(m+1) */
    L -= fn;                        /* 2*F(m+1) - F(m) = L(m) */

    return L + 1;                   /* a(n) = L(4n+1)+1 */
}
