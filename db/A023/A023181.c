/*
 * A023181  Numbers k such that Fibonacci(k) == 55 (mod k).
 * https://oeis.org/A023181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that Fibonacci(k) ≡ 55 (mod k)
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute Fibonacci(n) and Fibonacci(n+1) modulo mod with fast doubling. */
static void fib_pair_mod(idx_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1 % mod;        /* F(1) = 1 */
        return;
    }

    uint64_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);

    /* c = F(2k) mod mod, d = F(2k+1) mod mod */
    uint64_t two_b = ((unsigned __int128)2 * b) % mod;
    uint64_t sub   = (two_b + mod - a) % mod;            /* (2*b - a) mod mod */
    uint64_t c     = ((unsigned __int128)a * sub) % mod;
    uint64_t d     = (((unsigned __int128)a * a) + ((unsigned __int128)b * b)) % mod;

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
    if (n == 0) return (seq_t)0;          /* offset is 1; n ≥ 1 in problems */

    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        uint64_t mod    = (uint64_t)k;
        uint64_t target = 55 % mod;        /* required residue */
        uint64_t fk, _;
        fib_pair_mod(k, mod, &fk, &_);
        if (fk == target) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
