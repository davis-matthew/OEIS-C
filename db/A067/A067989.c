/*
 * A067989  Row sums of triangle A067979; also of triangle A067990.
 * https://oeis.org/A067989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*((3*n+1)*F(n‑1)/2 + (2*n+1)*F(n)), where F(k) is
 *            the k‑th Fibonacci number (F(0)=0,F(1)=1,F(‑1)=1)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast doubling algorithm to compute (F(n),F(n+1)) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1), k = n/2 */

    seq_t c = a * (((seq_t)2 * b) - a);           /* F(2k)   = F(k)*(2*F(k+1)-F(k))   */
    seq_t d = a * a + b * b;                      /* F(2k+1) = F(k)^2 + F(k+1)^2      */

    if ((n & 1) == 0) {           /* n is even */
        *fn  = c;
        *fn1 = d;
    } else {                      /* n is odd  */
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Fibonacci numbers */
    seq_t fn, fn1, fnm1, fnm1_next;
    fib_pair(n, &fn, &fn1);                /* fn  = F(n),        fn1 = F(n+1)   */
    fib_pair(n - 1, &fnm1, &fnm1_next);   /* fnm1 = F(n-1),    fnm1_next = F(n) */

    /* Compute (3*n+1)*F(n‑1) */
    seq_t term1 = (seq_t)(3 * n + 1) * fnm1;

    /* Compute 2*(2*n+1)*F(n) */
    seq_t term2 = (seq_t)2 * (seq_t)(2 * n + 1) * fn;

    /* Numerator of the inner parentheses */
    seq_t num = term1 + term2;

    /* Final result: (n+2)*num/2 */
    return (seq_t)(n + 2) * num / 2;
}
