/*
 * A054963  Number of cells in the first column of all directed column-convex
 *          polyominoes of area n+1.
 * https://oeis.org/A054963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(2n) + Fib(2n+2) - n (where Fib(0)=0, Fib(1)=1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 981 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Fibonacci numbers by fast doubling.
   fn  = Fib(n)
   fn1 = Fib(n+1) */
static void fib_doubling(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;                        /* a = Fib(k),  b = Fib(k+1) where k = n/2 */
    fib_doubling(n >> 1, &a, &b);

    seq_t two_b = b + b;                /* 2*Fib(k+1) */
    seq_t c = a * (two_b - a);          /* Fib(2k)      */
    seq_t d = a * a + b * b;            /* Fib(2k+1)    */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* First compute Fib(n) and Fib(n+1) */
    seq_t fn, fn1;
    fib_doubling(n, &fn, &fn1);

    seq_t two_fn1 = fn1 + fn1;          /* 2*Fib(n+1) */
    seq_t fib_2n  = fn * (two_fn1 - fn); /* Fib(2n)   */
    seq_t fib_2n1 = fn1 * fn1 + fn * fn;/* Fib(2n+1) */
    seq_t fib_2n2 = fib_2n1 + fib_2n;   /* Fib(2n+2) */

    seq_t res = fib_2n + fib_2n2 - (seq_t)n;
    return res;
}
