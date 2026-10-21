/*
 * A089508  Solution to a binomial problem together with companion sequence
 *          A081016(n-1).
 * https://oeis.org/A089508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(2*n) * F(2*n+1) - 1, where F(k) is the k-th Fibonacci
 *            number
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling Fibonacci: returns F(n) in *fn and F(n+1) in *fn1 */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);               /* a = F(k),  b = F(k+1), k = n/2 */
    seq_t two_b = b;
    two_b += b;                              /* 2*F(k+1) */
    two_b -= a;                              /* 2*F(k+1) - F(k) >= 0 */

    seq_t c = a * two_b;                     /* F(2k)      */
    seq_t aa = a * a;
    seq_t bb = b * b;
    seq_t d = aa + bb;                       /* F(2k+1)    */

    if ((n & 1) == 0) {                     /* n even      */
        *fn  = c;
        *fn1 = d;
    } else {                                 /* n odd       */
        seq_t cd = c + d;                    /* F(2k+2) = F(2k+1)+F(2k) */
        *fn  = d;
        *fn1 = cd;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* undefined for n=0, but return 0 safely */
    idx_t m = n * 2;                         /* compute 2n   */
    seq_t f_m, f_m1;
    fib_pair(m, &f_m, &f_m1);                /* f_m  = F(2n), f_m1 = F(2n+1) */
    seq_t prod = f_m * f_m1;
    prod -= (seq_t)1;
    return prod;
}
