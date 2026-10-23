/*
 * A186025  a(n) = 0^n + 1 - F(n-1)^2 - F(n)^2, where F = A000045.
 * https://oeis.org/A186025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0^n + 1 - F(n-1)^2 - F(n)^2 * where F is the Fibonacci
 *            sequence (F(0)=0, F(1)=1). * 0^0 is defined as 1. For n>0, 0^n =
 *            0. Negative index * for Fibonacci: F(-1)=1, but the formula only
 *            uses F(n-1) * of n=0, handled explicitly below
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 23, i64 n <= 46, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast doubling algorithm for Fibonacci numbers.
 * Computes F(n) and F(n+1) into *fn and *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;   /* F(0) */
    seq_t b = 1;   /* F(1) */

    /* iterate over bits of n from most significant to least */
    for (int i = 63; i >= 0; --i)
    {
        /* Compute F(2k) and F(2k+1) where current k corresponds
         * to the lower i+1 bits of n.  Variables c and d hold
         * F(2k) and F(2k+1) respectively. */
        seq_t t = a * ((b << 1) - a);   /* F(2k) */
        seq_t u = a * a + b * b;        /* F(2k+1) */

        if ((n >> i) & 1ULL)  /* current bit is 1 -> double and add */
        {
            a = u;
            b = t + u;
        }
        else /* current bit is 0 -> just double */
        {
            a = t;
            b = u;
        }
    }

    *fn  = a;  /* F(n)   */
    *fn1 = b;  /* F(n+1) */
}

seq_t a(idx_t n)
{
    /* Handle n=0 separately because F(-1) is not needed here. */
    if (n == 0)
        return 1;  /* 0^0 + 1 - F(-1)^2 - F(0)^2 = 1 */

    /* For n>=1 compute F(n-1) and F(n). */
    seq_t f_prev, f_curr;
    fib_pair(n - 1, &f_prev, &f_curr);  /* f_prev = F(n-1), f_curr = F(n) */

    /* a(n) = 1 - F(n-1)^2 - F(n)^2  (since 0^n = 0 for n>0) */
    return (seq_t)1 - f_prev * f_prev - f_curr * f_curr;
}
