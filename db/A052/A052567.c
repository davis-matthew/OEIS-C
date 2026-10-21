/*
 * A052567  E.g.f.: (1-x)^2/(1-3*x+x^2).
 * https://oeis.org/A052567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 (n=0), a(n) = n! * Fibonacci(2n) (n>0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fibonacci is defined by F(0)=0, F(1)=1, F(k+1)=F(k)+F(k-1).  */
/* The implementation uses a fast‑doubling algorithm for Fibonacci and
 * an iterative factorial computation.  All operations are performed
 * on seq_t, the integer type supplied by the build.  */

/* Compute (F(n), F(n+1)) using fast doubling */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;   /* F(0) */
    seq_t b = 1;   /* F(1) */

    /* Find most significant bit of n */
    idx_t temp = n;
    int msb = 0;
    while (temp > 0)
    {
        temp >>= 1;
        msb++;
    }

    for (int i = msb - 1; i >= 0; --i)
    {
        /* Loop invariant: a = F(k), b = F(k+1) for current k
           The standard doubling formulas:
           c = F(2k)   = a * (2*b - a)
           d = F(2k+1) = a*a + b*b
        */
        seq_t two_b  = b << 1;           /* 2 * F(k+1) */
        seq_t temp1  = two_b - a;        /* 2*F(k+1) - F(k)   (non‑negative) */
        seq_t c      = a * temp1;        /* F(2k) */
        seq_t d      = a * a + b * b;    /* F(2k+1) */

        if ((n >> i) & 1)
        {
            a = d;      /* k -> 2k+1 */
            b = c + d;  /* F(2k+2) */
        }
        else
        {
            a = c;      /* k -> 2k   */
            b = d;      /* F(2k+1)   */
        }
    }

    *fn  = a;
    *fn1 = b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute F(n) and F(n+1) */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);

    /* Compute F(2n) = F(n) * (2*F(n+1) - F(n)) */
    seq_t two_fn1 = fn1 << 1;
    seq_t temp    = two_fn1 - fn;   /* 2*F(n+1) - F(n) */
    seq_t f2n     = fn * temp;      /* F(2n) */

    /* a(n) = n! * F(2n) */
    return fact * f2n;
}
