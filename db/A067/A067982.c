/*
 * A067982  Convolution of L(n+1) = A000204(n+1), n>=0, with L(n+4), n>=0.
 * https://oeis.org/A067982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n+7)*F(n+1)+7*(n+1)*F(n) where F is the Fibonacci
 *            sequence F(0)=0, F(1)=1
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t – used only if needed by idx_t */

/* Fast doubling to compute F(n) and F(n+1) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);

    /* c = F(2k) = F(k) * (2*F(k+1) - F(k)) */
    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n) {
    seq_t Fn, Fn1;
    fib_pair(n, &Fn, &Fn1);          /* Fn = F(n),   Fn1 = F(n+1) */

    seq_t term1 = (seq_t)(11 * n + 7) * Fn1;   /* (11*n+7)*F(n+1) */
    seq_t term2 = (seq_t)(7 * (n + 1)) * Fn;   /* 7*(n+1)*F(n)   */

    return term1 + term2;
}
