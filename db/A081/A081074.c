/*
 * A081074  a(n) = Fibonacci(4n)-3, or Fibonacci(2n-2)*Lucas(2n+2).
 * https://oeis.org/A081074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n) - 3, n >= 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast doubling Fibonacci: returns F(n) as seq_t.
   Works for any seq_t that supports +, *, << and - operations. */
static seq_t fib(idx_t n)
{
    if (n == 0) return 0;          /* F(0) */

    /* a = F(k), b = F(k+1); start with k = 0 */
    seq_t a = 0;   /* F(0) */
    seq_t b = 1;   /* F(1) */

    /* find highest set bit (n > 0) */
    idx_t high = 63 - __builtin_clzll(n);

    for (idx_t i = high; ; --i) {
        /* Loop invariant: a = F(k), b = F(k+1) */
        seq_t two_b = b + b;                /* 2*b  */
        seq_t d = a * (two_b - a);          /* F(2k)   */
        seq_t e = a * a + b * b;            /* F(2k+1) */

        if ( (n >> i) & 1 ) {              /* if bit is 1, go to k+1 */
            a = e;
            b = d + e;
        } else {                           /* else stay at k     */
            a = d;
            b = e;
        }

        if (i == 0) break;                 /* processed lowest bit */
    }

    return a;           /* F(n) */
}

seq_t a(idx_t n)
{
    /* Compute F(4*n) and subtract 3.
       n is at least 1, so 4*n fits in idx_t for the limits considered. */
    idx_t four_n = n * 4ULL;
    seq_t fib_val = fib( four_n );
    return fib_val - 3;
}
