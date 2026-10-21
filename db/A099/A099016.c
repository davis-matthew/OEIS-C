/*
 * A099016  a(n) = 3*L(2*n)/5 - (-1)^n/5, where L = A000032.
 * https://oeis.org/A099016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*F(n)^2 + F(n)*F(n-1) + F(n-1)^2 where F(k) is the k‑th
 *            Fibonacci number (F(0)=0, F(1)=1). This satisfies a(0)=1,
 *            a(1)=2, a(2)=4, and a(n)=2*a(n-1)+2*a(n-2)-a(n-3)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> // for size_t (not used but harmless in C++)

/* Compute Fibonacci pair (F(n), F(n+1)) by fast doubling. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    idx_t k = n >> 1;            /* n / 2 */
    seq_t a, b;
    fib_pair(k, &a, &b);         /* a = F(k), b = F(k+1) */

    seq_t two_b = b << 1;       /* 2*F(k+1) */
    seq_t c = a * (two_b - a);  /* F(2k)   = F(k) * (2*F(k+1) - F(k)) */
    seq_t d = a * a + b * b;    /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) {         /* n is even */
        *fn  = c;               /* F(2k)   */
        *fn1 = d;               /* F(2k+1) */
    } else {                    /* n is odd  */
        *fn  = d;               /* F(2k+1) */
        *fn1 = c + d;           /* F(2k+2) = F(2k) + F(2k+1) */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */
    if (n == 1) return 2;          /* a(1) = 2 */

    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);        /* fn  = F(n),  fn1 = F(n+1) */
    seq_t fnm1 = fn1 - fn;         /* F(n-1) = F(n+1) - F(n) */

    seq_t term1 = fn;
    term1 *= term1;                /* fn^2 */
    term1 *= 2;                    /* 2*fn^2 */

    seq_t term2 = fn;
    term2 *= fnm1;                 /* fn*fnm1 */

    seq_t term3 = fnm1;
    term3 *= fnm1;                 /* fnm1^2 */

    seq_t result = term1;
    result += term2;
    result += term3;

    return result;
}
