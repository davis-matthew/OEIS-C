/*
 * A214738  a(n) is the least m > 0 such that n-m divides Fibonacci(n)+m.
 * https://oeis.org/A214738
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min m>0 such that (n-m) | (F_n + m) where F_n is the nth
 *            Fibonacci number (F_0=0,F_1=1)
 * Range:     u8 n <= 267, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t fib_mod(uint64_t n, uint64_t mod)
{
    /* Return F_n mod mod.  mod>0.  Handle mod==1 early. */
    if (mod == 1) return 0;
    uint64_t a = 0;        /* F(0) */
    uint64_t b = 1;        /* F(1) */
    /* Process bits of n from most significant to least significant. */
    for (int i = 63 - __builtin_clzll(n); i >= 0; --i) {
        /* c = F(2k)   = F(k) * (2*F(k+1) - F(k))   (mod mod) */
        uint64_t t1 = (2 * b) % mod;
        if (t1 >= a) t1 -= a; else t1 = t1 + mod - a;
        uint64_t c = (a * t1) % mod;
        /* d = F(2k+1) = F(k)^2 + F(k+1)^2 (mod mod) */
        uint64_t d = ((a * a) % mod + (b * b) % mod) % mod;
        if ((n >> i) & 1) {
            a = d;
            b = (c + d) % mod;
        } else {
            a = c;
            b = d;
        }
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined below offset */

    /* Check candidate divisors d = n-m from n-1 down to 1. */
    for (idx_t d = n - 1; d >= 2; --d) {
        uint64_t f = fib_mod(n, d);       /* F_n mod d */
        uint64_t val = (f + n) % d;       /* (F_n + n) mod d  == (F_n + m) mod d */
        if (val == 0)                     /* d | (F_n + m)   -> m = n-d */
            return (seq_t)(n - d);
    }
    /* d == 1 always works: m = n-1 */
    return (seq_t)(n - 1);
}
