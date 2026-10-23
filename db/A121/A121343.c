/*
 * A121343  a(n) = Fibonacci(n) mod n(n+1)/2.
 * https://oeis.org/A121343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod (n*(n+1)/2)
 * Range:     u8 n <= 21, u16 n <= 376, u32 n <= 1379, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling algorithm to compute Fib(n) modulo mod */
static void fib_pair_rec(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair_rec(n >> 1, mod, &a, &b);           /* F(k), F(k+1) for k = n/2 */
    seq_t two_b          = b * 2;
    seq_t two_b_minus_a  = (two_b + mod - a) % mod;   /* (2*b - a) mod mod */
    seq_t c = (a * two_b_minus_a) % mod;               /* F(2k)    */
    seq_t d = (a * a + b * b) % mod;                  /* F(2k+1)  */
    if (n & 1) {
        *fn  = d % mod;               /* F(n)   = F(2k+1)      */
        *fn1 = (c + d) % mod;        /* F(n+1) = F(2k)+F(2k+1) */
    } else {
        *fn  = c % mod;              /* F(n)   = F(2k)    */
        *fn1 = d % mod;              /* F(n+1) = F(2k+1)  */
    }
}

static seq_t fib_mod(idx_t n, seq_t mod)
{
    if (mod == 0) return 0;                     /* handles n = 0 */
    seq_t f0, f1;
    fib_pair_rec(n, mod, &f0, &f1);
    return f0 % mod;                            /* already reduced */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* a(0) = 0 */
    seq_t m = (seq_t)n * ((seq_t)n + 1) / 2;    /* triangular(n) in seq_t */
    return fib_mod(n, m);                       /* Fibonacci(n) mod m */
}
