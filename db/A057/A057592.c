/*
 * A057592  a(n) = Fibonacci(n+1)^2 + 4*Fibonacci(n).
 * https://oeis.org/A057592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+1)^2 + 4*Fibonacci(n)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Fibonacci numbers with fast doubling (O(log n)) */
/* Works for arbitrary-precision seq_t and 64‑bit seq_t */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);      /* a = F(k), b = F(k+1) where k = n/2 */

    /* c = F(2k)   = a * (2*b - a) */
    /* d = F(2k+1) = a*a + b*b       */
    seq_t two_b = b * (seq_t)2;
    seq_t t     = two_b - a;        /* 2*b - a  (always non-negative) */
    seq_t c     = a * t;
    seq_t d     = a * a + b * b;

    if ((n & 1) == 0) {            /* n even: F(n) = c, F(n+1) = d   */
        *fn  = c;
        *fn1 = d;
    } else {                       /* n odd:  F(n) = d, F(n+1) = c + d */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;                  /* fn  = Fibonacci(n)   */
    fib_pair(n, &fn, &fn1);         /* fn1 = Fibonacci(n+1) */

    seq_t term1 = fn1 * fn1;        /* F(n+1)^2   */
    seq_t term2 = (seq_t)4 * fn;    /* 4*F(n)     */
    return term1 + term2;
}
