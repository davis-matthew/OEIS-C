/*
 * A022351  Fibonacci sequence beginning 0, 17.
 * https://oeis.org/A022351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*A000045(n) (Fibonacci sequence starting 0,1)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, but seq_t/idx_t are supplied externally */

/* Fast‑doubling algorithm: computes F(n) and F(n+1) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);            /* a = F(k),  b = F(k+1)   */

    /* c = F(2k)   = F(k) * (2*F(k+1) – F(k))   */
    seq_t two_b_minus_a = (b << 1) - a;    /* 2*b – a  is always non‑negative   */
    seq_t c = a * two_b_minus_a;

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2            */
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Public interface required by the build system */
seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);        /* fn = F(n) */
    return (seq_t)17 * fn;         /* a(n) = 17 * F(n) */
}
