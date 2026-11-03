/*
 * A079345  a(n) = Fibonacci(n) mod 16.
 * https://oeis.org/A079345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 16. Fast doubling with modular reduction
 *            performed before any subtraction to avoid negative intermediate
 *            results
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 110 terms through n = 933 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast doubling algorithm for Fibonacci modulo 16.  fn holds
   F(n) and fn1 holds F(n+1).  All arithmetic is done modulo 16.  */
static void fib_mod_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;                     /* a = F(k), b = F(k+1) for k = n >> 1 */
    fib_mod_pair(n >> 1, &a, &b);

    const seq_t mod = 16;

    /* Compute t1 = (2*F(k+1) - F(k)) mod 16, ensuring no negative value */
    seq_t t1 = ((b * 2) % mod + mod - a) % mod;

    /* c = F(2k)   = F(k) * (2*F(k+1) - F(k))  mod 16 */
    seq_t c = (a * t1) % mod;

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2            mod 16 */
    seq_t d = ((a * a) % mod + (b * b) % mod) % mod;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_mod_pair(n, &fn, &fn1);
    return fn;            /* fn is already reduced modulo 16 */
}
