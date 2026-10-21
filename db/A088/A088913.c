/*
 * A088913  a(n) = (Lucas(2n) + Lucas(2n+1)*phi)/kappa(phi/Fibonacci(4n))
 *          where kappa(x) is the sum of successive remainders in computing
 *          the Euclidean algorithm for (1,x).
 * https://oeis.org/A088913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Lucas(6*n) - Lucas(2*n) where Lucas(k) = Fibonacci(k-1) +
 *            Fibonacci(k+1) This implementation uses fast doubling for
 *            Fibonacci and an O(log n) computation of each Lucas number
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   mulseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mulseq.h"

/* Fast doubling: compute F(n) and F(n+1).
   The invariant maintains (*fn  = F(n), *fn1 = F(n+1)). */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);      /* a = F(k), b = F(k+1), k = n/2 */

    seq_t two_b = mulseq(b, 2);        /* 2*b */
    seq_t c = mulseq(a, (two_b - a));  /* F(2k)   = a*(2*b - a)   */
    seq_t d = mulseq(a, a) + mulseq(b, b); /* F(2k+1) = a^2 + b^2   */

    if ((n & 1) == 0) {          /* n = 2k */
        *fn  = c;
        *fn1 = d;
    } else {                     /* n = 2k + 1 */
        *fn  = d;
        *fn1 = c + d;            /* F(2k+2) */
    }
}

/* Compute Lucas(k) = 2*F(k+1) - F(k) */
static seq_t lucas(idx_t k)
{
    seq_t f_k, f_k1;
    fib_pair(k, &f_k, &f_k1);        /* f_k = F(k), f_k1 = F(k+1) */
    return mulseq(f_k1, 2) - f_k;    /* 2*F(k+1) - F(k) */
}

seq_t a(idx_t n)
{
    idx_t six_n = n * 6;
    idx_t two_n = n * 2;

    seq_t l6 = lucas(six_n);
    seq_t l2 = lucas(two_n);

    return l6 - l2;
}
