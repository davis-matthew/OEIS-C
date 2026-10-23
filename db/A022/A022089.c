/*
 * A022089  Fibonacci sequence beginning 0, 6.
 * https://oeis.org/A022089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * F(n), where F(0)=0, F(1)=1 and F(k)=F(k-1)+F(k-2) for
 *            k>1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0)
    {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);               /* compute (F(m),F(m+1)) for m=n/2 */

    seq_t two_b = b + b;                    /* 2*F(m+1) */
    seq_t c = a * (two_b - a);              /* F(2m)  = F(m)*(2*F(m+1)-F(m))   */
    seq_t d = a * a + b * b;                /* F(2m+1)=F(m)^2+F(m+1)^2          */

    if ((n & 1) == 0)
    {
        *fn  = c;                           /* F(2m)  */
        *fn1 = d;                           /* F(2m+1)*/
    }
    else
    {
        *fn  = d;                           /* F(2m+1)*/
        *fn1 = c + d;                       /* F(2m+2)=F(2m)+F(2m+1) */
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);                 /* fn = F(n) */
    return fn * 6;                          /* a(n) = 6 * F(n) */
}
