/*
 * A069403  a(n) = 2*Fibonacci(2*n+1) - 1.
 * https://oeis.org/A069403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fibonacci(2*n+1) - 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast‑doubling Fibonacci computation. */

#include <stdint.h>

/* Compute Fibonacci(n) and Fibonacci(n+1) into fn and fn1.  */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0, b = 1;   /* a = F(0), b = F(1) */

    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    /* Find most significant set bit of n.  */
    int msb = 63;
    while (msb > 0 && !(n & ((idx_t)1 << msb)))
        msb--;

    for (int i = msb; i >= 0; --i) {
        /* c = F(2k)   = F(k) * (2*F(k+1) – F(k))   */
        /* d = F(2k+1) = F(k)^2 + F(k+1)^2           */
        seq_t two_b = b << 1;
        seq_t t1    = two_b - a;
        seq_t c     = a * t1;
        seq_t d     = a * a + b * b;

        if ((n >> i) & 1) {
            a = d;
            b = c + d;
        } else {
            a = c;
            b = d;
        }
    }

    *fn  = a;
    *fn1 = b;
}

seq_t a(idx_t n)
{
    /* Compute F(2*n+1) */
    idx_t k = n * 2 + 1;
    seq_t fk, fk_next;
    fib_pair(k, &fk, &fk_next);   /* fk holds Fibonacci(k) */

    /* a(n) = 2*fk - 1 */
    seq_t res = fk + fk;   /* multiply by 2 */
    res -= (seq_t)1;
    return res;
}
