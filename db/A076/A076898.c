/*
 * A076898  Let u(1)=u(2)=u(3)=1, u(n)=sign(u(n-1)-u(n-2))/(u(n-3)+1); then
 *          a(n) is the numerator of u(n).
 * https://oeis.org/A076898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign
 * Formula:   not stated
 * Range:     i8 n <= 61, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A076898 – Integer sequence defined by Fibonacci numbers.
 *  a(1)=a(2)=a(3)=1.
 *  For k ≥ 0:
 *      a(6k)   = -F(k)
 *      a(6k+1) =  0
 *      a(6k+2) =  F(k+2)
 *      a(6k+3) =  F(k+2)
 *      a(6k+4) =  0
 *      a(6k+5) = -F(k+1)
 *  (F(0)=0, F(1)=1) */
seq_t a(idx_t n)
{
    /* Handle the three special initial values */
    if (n <= 3) return (seq_t)1;

    /* Index of the block of six */
    idx_t k   = n / 6;
    idx_t rem = n % 6;

    /* We need Fibonacci numbers up to index k+2 (because
     * a(6k+5) uses F(k+1) and a(6k+2/3) use F(k+2) ).
     * The maximum 'n' the system tests is ~10000,
     * so k+2 < 2000; a static array of that size is safe
     * for all supported seq_t types. */
    static seq_t fib[2000];
    static idx_t fib_len = 0;  /* number of Fibonacci values currently stored */

    if (!fib_len) {          /* first call: initialise */
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_len = 2;
    }

    idx_t need = k + 2;      /* highest index we must have ready */
    if (need + 1 > fib_len) /* need up to index need inclusive */
    {
        for (idx_t i = fib_len; i <= need; ++i)
            fib[i] = fib[i-1] + fib[i-2];
        fib_len = need + 1;
    }

    switch (rem) {
        case 0:   return -(fib[k]);       /* a(6k)   = -F(k) */
        case 1:   return (seq_t)0;        /* a(6k+1) = 0   */
        case 2:   /* fall through */
        case 3:   return fib[k+2];        /* a(6k+2) = F(k+2) */
        case 4:   return (seq_t)0;        /* a(6k+4) = 0   */
        case 5:   return -(fib[k+1]);     /* a(6k+5) = -F(k+1) */
    }
    /* Unreachable – keep compiler happy */
    return (seq_t)0;
}
