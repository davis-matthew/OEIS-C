/*
 * A152727  Smallest positive non-divisor of the n-th Fibonacci number
 *          (A000045).
 * https://oeis.org/A152727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer m ≥ 2 such that F(n) mod m ≠ 0 where
 *            F(n) is the n‑th Fibonacci number (F(1)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling modular Fibonacci for modulus mod.
   Returns F(n) mod mod.  n is 1‑based (F(1)==1).  */
static uint64_t fib_mod_u64(idx_t n, uint64_t mod)
{
    /* Handle trivial modulus */
    if (mod == 1)
        return 0;

    uint64_t a = 0;      /* F(0) */
    uint64_t b = 1;      /* F(1) */

    /* Process bits of n from most significant to least */
    idx_t mask = (idx_t)1 << (63 - __builtin_clzll(n));

    while (mask) {
        /* c = a * (2*b - a) mod mod */
        uint64_t two_b = ((unsigned __int128)b << 1) % mod;
        uint64_t two_b_minus_a = (two_b + mod - a) % mod;
        uint64_t c = ((unsigned __int128)a * two_b_minus_a) % mod;

        /* d = a*a + b*b mod mod */
        uint64_t d = (((unsigned __int128)a * a) + ((unsigned __int128)b * b)) % mod;

        if (n & mask) {
            a = d;
            b = (c + d) % mod;
        } else {
            a = c;
            b = d;
        }
        mask >>= 1;
    }

    return a % mod;   /* F(n) mod mod */
}

seq_t a(idx_t n)
{
    /* As per OEIS offset, n ≥ 1. */
    if (n < 1)
        return 0;   /* undefined, but harmless */

    for (uint64_t m = 2;; ++m) {
        if (fib_mod_u64(n, m) != 0)
            return (seq_t)m;
    }
}
