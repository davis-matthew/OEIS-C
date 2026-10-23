/*
 * A192967  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*Fib(n+1) - n - 2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast‑doubling to compute Fibonacci numbers in O(log n). */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t f0, f1;
    fib_pair(n >> 1, &f0, &f1);          /* F(k), F(k+1) with k = n/2 */

    /* Compute: c = F(k) * (2*F(k+1) - F(k)) */
    /*        d = F(k)^2 + F(k+1)^2 */
    seq_t two_b = f1 * (seq_t)2;
    seq_t c = f0 * (two_b - f0);
    seq_t d = f0 * f0 + f1 * f1;

    if ((n & 1) == 0) {                  /* n even: F(n)=c, F(n+1)=d */
        *fn = c;
        *fn1 = d;
    } else {                              /* n odd:  F(n)=d, F(n+1)=c+d */
        *fn = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n + (idx_t)1, &fn, &fn1);           /* fn = F(n+1) */
    seq_t result = fn * (seq_t)3 - (seq_t)n - (seq_t)2;
    return result;
}
