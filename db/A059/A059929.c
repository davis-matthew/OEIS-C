/*
 * A059929  a(n) = Fibonacci(n)*Fibonacci(n+2).
 * https://oeis.org/A059929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) * Fibonacci(n+2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast doubling Fibonacci:
   fib_pair(n, &F(n), &F(n+1)) */ 
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) { /* F(0)=0, F(1)=1 */
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);           /* a = F(k), b = F(k+1), k = n/2 */

    seq_t two_b = (seq_t)2 * b;          /* 2*F(k+1) */
    seq_t c     = a * (two_b - a);       /* F(2k) = F(k) * (2*F(k+1) - F(k)) */
    seq_t d     = a * a + b * b;         /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) {                  /* n even: return (F(2k), F(2k+1)) */
        *fn  = c;
        *fn1 = d;
    } else {                             /* n odd: return (F(2k+1), F(2k+2)) */
        *fn  = d;
        *fn1 = c + d;                    /* F(2k+2) = F(2k) + F(2k+1) */
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);              /* fn  = F(n),   fn1 = F(n+1) */
    seq_t fn2 = fn + fn1;                /* F(n+2) = F(n) + F(n+1) */
    return fn * fn2;                     /* a(n) = F(n) * F(n+2) */
}
