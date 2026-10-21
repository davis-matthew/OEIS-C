/*
 * A061593  Number of ways to place 2n nonattacking kings on a 4 X 2n
 *          chessboard.
 * https://oeis.org/A061593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (17*n-109)*3^n + 2*Fibonacci(2*n+10)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fibonacci is defined by F(0)=0, F(1)=1, with the usual recurrence. */

#include <stddef.h>   /* for NULL if needed, not actually used here */

/* Fast doubling: compute F(n) and F(n+1). */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b); /* a = F(k), b = F(k+1) */

    /* c = F(2k)   = a * (2*b - a) */
    seq_t tmp = (b << 1) - a;  /* 2*b - a  (non‑negative for Fibonacci indices) */
    c = a * tmp;

    /* d = F(2k+1) = a*a + b*b   */
    d = a * a + b * b;

    if (n & 1) {
        *fn  = d;              /* F(2k+1) */
        *fn1 = c + d;          /* F(2k+2) */
    } else {
        *fn  = c;              /* F(2k)   */
        *fn1 = d;              /* F(2k+1) */
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n==0 return 0. */
    if (n == 0) return 0;

    /* Compute (17*n-109) as a signed 64‑bit value – it stays small for n <= 10000. */
    int64_t coeff = (int64_t)17 * (int64_t)n - 109;
    int64_t abs_coeff = coeff >= 0 ? coeff : -coeff;

    /* 3^n by binary exponentiation. */
    seq_t power3 = 1;
    seq_t base    = 3;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) power3 *= base;
        base *= base;
        exp >>= 1;
    }

    /* Fibonacci(2*n+10).  We only need F(2*n+10); the second value of the pair is unused. */
    seq_t dummy, fib_val;
    fib_pair(2 * n + 10, &fib_val, &dummy);
    seq_t two_fib = fib_val + fib_val;   /* 2 * Fibonacci(2*n+10) */

    /* term1 = abs(coeff) * 3^n   (unsigned multiplication). */
    seq_t term1_abs = power3 * (seq_t)abs_coeff;

    /* Assemble the final result.  The sign of coeff decides whether to add or subtract. */
    seq_t result;
    if (coeff >= 0)
        result = term1_abs + two_fib;
    else
        result = two_fib - term1_abs;

    return result;
}
