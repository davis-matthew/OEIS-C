/*
 * A084086  a(n) = Fibonacci(2*n+1) + 2*Fibonacci(2*n-1) - 2^n - [n = 0],
 *          where [b] is the Iverson bracket of b.
 * https://oeis.org/A084086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+1) + 2*Fibonacci(2*n-1) - 2^n - [n=0]
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

/* Fast doubling Fibonacci is not used; an iterative version suffices for n ≤ 10000. */
static seq_t fib_fast(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t a = 0;   /* F(k)   */
    seq_t b = 1;   /* F(k+1) */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        seq_t c = a + b; /* next Fibonacci */
        a = b;
        b = c;
    }
    return b;
}

/* Compute 2^n iteratively using seq_t arithmetic. */
static seq_t pow2(idx_t n)
{
    seq_t r = 1;
    while (n--) {
        r += r; /* r *= 2 */
    }
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;          /* a(0) defined explicitly */
    }

    /* Compute required Fibonacci values. */
    seq_t f_hi = fib_fast(2 * n + 1);     /* Fibonacci(2n+1) */
    seq_t f_lo = fib_fast(2 * n - 1);     /* Fibonacci(2n-1) */

    /* Compute 2^n. */
    seq_t p2 = pow2(n);

    /* a(n) = F(2n+1) + 2*F(2n-1) - 2^n */
    seq_t result = f_hi + f_lo + f_lo;    /* f_hi + 2*f_lo */
    result -= p2;
    return result;
}
