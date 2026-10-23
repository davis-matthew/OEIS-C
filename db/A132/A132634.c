/*
 * A132634  a(n) = Fibonacci(n) mod n^2.
 * https://oeis.org/A132634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) mod n^2
 * Range:     u8 n <= 17, u16 n <= 256, u32 n <= 9965, u64 n <= 9965,
 *            u128 n <= 9965, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Fast doubling algorithm to compute Fibonacci(n) and Fibonacci(n+1)
   modulo mod.  Operates entirely on 64‑bit unsigned integers; the
   intermediate values stay below 2^64 for all n that appear in the
   sequence. */
static void fib_mod(idx_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }

    uint64_t a, b;
    fib_mod(n >> 1, mod, &a, &b);               /* a = F(k),  b = F(k+1) */

    uint64_t two_b = (b << 1) % mod;            /* 2*b  (mod mod) */
    uint64_t tmp   = (two_b + mod - a) % mod;   /* 2*b - a (mod mod) */
    uint64_t c     = (a * tmp) % mod;           /* F(2k)                */
    uint64_t d     = ( (a * a) % mod + (b * b) % mod ) % mod; /* F(2k+1) */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* Return the A132634 sequence value for index n (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    uint64_t mod = (uint64_t)n * (uint64_t)n;   /* n^2, fits in 64‑bit for all n here */
    uint64_t fn, unused;
    fib_mod(n, mod, &fn, &unused);

    return (seq_t)fn;
}
