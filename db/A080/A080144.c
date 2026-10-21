/*
 * A080144  a(n) = F(4)*F(n)*F(n+1) + F(5)*F(n+1)^2 if n odd, a(n) =
 *          F(4)*F(n)*F(n+1) + F(5)*F(n+1)^2 - F(5) if n even, where F(n) is
 *          the n-th Fibonacci number (A000045).
 * https://oeis.org/A080144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*Fibonacci(n+3)^2 - 5 - 3*(-1)^n)/2 (where
 *            Fibonacci(0)=0, Fibonacci(1)=1) This variant is equivalent to
 *            the definitions in the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Fast doubling Fibonacci: fills fn = F(n), fn1 = F(n+1) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1), k = n/2 */

    seq_t two_b = b + b;               /* 2*F(k+1) */
    seq_t c = a * (two_b - a);         /* F(2k) */
    seq_t d = a * a + b * b;           /* F(2k+1) */

    if (n & 1) {                       /* odd n */
        *fn  = d;                      /* F(n) = d */
        *fn1 = c + d;                  /* F(n+1) = c + d */
    } else {                           /* even n */
        *fn  = c;
        *fn1 = d;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Get F(n+3) */
    seq_t fn, fn1;
    fib_pair(n + 3, &fn, &fn1);         /* fn = F(n+3) */

    /* temp = 2*fn^2 */
    seq_t sq = fn * fn;                 /* fn^2 */
    seq_t temp = sq + sq;               /* 2*fn^2 */

    /* Compute subtraction part: 5 + (n even ? 3 : -3) */
    seq_t sub = 5;
    if (n & 1) {                        /* n odd: subtract 5-3 = 2 */
        sub -= 3;                       /* sub = 2 */
    } else {                            /* n even: subtract 5+3 = 8 */
        sub += 3;                       /* sub = 8 */
    }

    /* numerator = temp - sub (always non-negative) */
    seq_t numer = temp - sub;

    /* Divide by 2 */
    return numer / 2;
}
