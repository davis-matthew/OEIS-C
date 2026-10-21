/*
 * A067985  Convolution of L(n+1) := A000204(n+1) (Lucas), n>=0, with L(n+7),
 *          n>=0.
 * https://oeis.org/A067985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (47*n+29)*F(n+1) + 29*(n+1)*F(n), where F(k) is the k‑th
 *            Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed by the build */

/* Fast doubling algorithm to compute (F(n), F(n+1)) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k),   b = F(k+1),  k = n/2 */

    seq_t two_b = b + b;                /* 2*F(k+1) */
    seq_t c = a * (two_b - a);          /* F(2k)   = F(k)*(2*F(k+1)-F(k)) */
    seq_t d = a * a + b * b;            /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t F_n, F_n1;
    fib_pair(n, &F_n, &F_n1);                 /* get F(n) and F(n+1) */

    seq_t coeff1 = (seq_t)47 * (seq_t)n + (seq_t)29;
    seq_t coeff2 = (seq_t)29 * (seq_t)(n + 1);

    return coeff1 * F_n1 + coeff2 * F_n;
}
