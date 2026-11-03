/*
 * A033889  a(n) = Fibonacci(4*n + 1).
 * https://oeis.org/A033889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n + 1) Recurrence: a(n) = 7*a(n-1) - a(n-2),
 *            a(0) = 1, a(1) = 5 This implementation uses fast‑doubling to
 *            compute the Fibonacci number efficiently for arbitrary n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 500 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast doubling recurrence:
   Fib(2k)   = Fib(k) * (2*Fib(k+1) - Fib(k))
   Fib(2k+1) = Fib(k+1)^2 + Fib(k)^2
   This routine returns (Fib(n), Fib(n+1)).
*/
static void fib_pair_doubling(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair_doubling(n >> 1, &a, &b);

    /* c = Fib(2k), d = Fib(2k+1) */
    seq_t c = a * ((b * 2) - a);
    seq_t d = a * a + b * b;

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
    /* We need Fib(4*n + 1). */
    idx_t m = 4 * n + 1;
    seq_t fn, fn1;
    fib_pair_doubling(m, &fn, &fn1);
    return fn;
}
