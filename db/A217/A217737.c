/*
 * A217737  a(n) = Fibonacci(n) mod n*(n+1).
 * https://oeis.org/A217737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) modulo n*(n+1)
 * Range:     u8 n <= 15, u16 n <= 272, u32 n <= 272, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void fib_mod_pair(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_mod_pair(n >> 1, mod, &a, &b);   /* a = F(k), b = F(k+1) */

    /* Ensure all intermediate results are taken modulo mod */
    seq_t two_b = b + b;                     /* 2*b            */
    seq_t tmp   = (two_b + mod - a) % mod;   /* (2*b - a) mod m */
    seq_t c     = (a * tmp) % mod;           /* a*(2*b - a)    */
    seq_t d     = ((a * a) + (b * b)) % mod; /* a² + b²        */

    if ((n & 1) == 0) {                     /* even n */
        *fn  = c;
        *fn1 = d;
    } else {                                 /* odd n  */
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)n * ((seq_t)n + 1);   /* n*(n+1) in seq_t */
    seq_t fn, fn1;
    fib_mod_pair(n, mod, &fn, &fn1);
    return fn % mod;                         /* F(n) mod mod    */
}
