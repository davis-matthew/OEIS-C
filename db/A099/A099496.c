/*
 * A099496  a(n) = (-1)^n * Fibonacci(2*n+1).
 * https://oeis.org/A099496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^n * Fibonacci(2*n+1)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 22, i64 n <= 45, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive fast‑doubling algorithm for Fibonacci numbers.
   It is independent of any external helper functions and works for
   any seq_t type that supports +, -, *, and assignment. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0)
    {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);                // a = F(k), b = F(k+1)
    seq_t c = a * ((b * 2) - a);             // F(2k)
    seq_t d = a * a + b * b;                 // F(2k+1)

    if ((n & 1) == 0)                       // n = 2k
    {
        *fn  = c;
        *fn1 = d;
    }
    else                                     // n = 2k+1
    {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Public entry point for sequence A099496 */
seq_t a(idx_t n)
{
    /* Compute Fib(2*n + 1) via fast doubling. */
    idx_t arg = (n << 1) + 1;                 // 2*n + 1
    seq_t fib_val;
    seq_t dummy;                              // F(n+1), not needed
    fib_pair(arg, &fib_val, &dummy);

    /* Apply the sign (-1)^n. */
    return (n & 1) ? -fib_val : fib_val;
}
