/*
 * A094966  Left-hand neighbors of Fibonacci numbers in Stern's diatomic
 *          series.
 * https://oeis.org/A094966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, less, nonn
 * Formula:   a(n) = Fibonacci(n) if n is even Fibonacci(n + 1) if n is odd
 *            (OEIS A094966)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 46, u64 n <= 92, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive fast‑doubling to compute Fibonacci numbers.
   It returns (F(n), F(n+1)) in fn and fn1.
   Works for any seq_t supporting +, -, * operators. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b; /* a = F(k), b = F(k+1) where k = n/2 */
    fib_pair(n >> 1, &a, &b);

    /* c = F(2k) = a * (2*b - a) */
    seq_t two_b = b + b;
    seq_t two_b_minus_a = two_b - a;
    seq_t c = a * two_b_minus_a;

    /* d = F(2k+1) = a*a + b*b */
    seq_t d = a * a + b * b;

    if (n & 1) {
        *fn  = d;
        *fn1 = c + d;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) or Fibonacci(n+1) according to n’s parity */
    if ((n & 1) == 0) {
        seq_t fn, fn1;
        fib_pair(n, &fn, &fn1);
        return fn;
    } else {
        seq_t fn, fn1;
        fib_pair(n + 1, &fn, &fn1);
        return fn;
    }
}
