/*
 * A127546  a(n) = F(n)^2 + F(n+1)^2 + F(n+2)^2, where F(n) denotes the n-th
 *          Fibonacci number.
 * https://oeis.org/A127546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)^2 + F(n+1)^2 + F(n+2)^2 where F(n) is the n‑th
 *            Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute (F(n), F(n+1)) by fast doubling.  Uses only
   addition, subtraction and multiplication on seq_t. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);

    /* F(2k)   = F(k) * (2*F(k+1) – F(k)) */
    seq_t b2 = b + b;          /* 2*F(k+1) */
    seq_t temp = b2 - a;       /* 2*F(k+1) – F(k) */
    c = a * temp;

    /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    d = a * a + b * b;

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
    fib_pair(n, &fn, &fn1);                /* fn = F(n) , fn1 = F(n+1) */
    seq_t fn2 = fn + fn1;                  /* F(n+2) = F(n) + F(n+1) */
    /* a(n) = F(n)^2 + F(n+1)^2 + F(n+2)^2 */
    return fn * fn + fn1 * fn1 + fn2 * fn2;
}
