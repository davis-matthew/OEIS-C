/*
 * A104714  Greatest common divisor of a Fibonacci number and its index.
 * https://oeis.org/A104714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(Fibonacci(n), n)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling method for Fibonacci modulo mod.
   Computes F(n) and F(n+1) modulo mod and stores them in *fn and *fn1. */
static inline void fib_pair(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0 % mod;   /* 0 mod mod is 0 (mod may be 0, but we handle n==0 before calling) */
        *fn1 = 1 % mod;
        return;
    }
    uint64_t a, b;
    fib_pair(n >> 1, mod, &a, &b);          /* a = F(k), b = F(k+1), where k = n/2 */
    uint64_t t1 = ((2 * b) % mod + mod - a) % mod;          /* (2*F(k+1) - F(k)) mod mod */
    uint64_t c  = (unsigned __int128)a * t1 % mod;          /* F(2k)     */
    uint64_t d  = ((unsigned __int128)a * a + (unsigned __int128)b * b) % mod; /* F(2k+1) */

    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;            /* F(2k+1), F(2k+2) */
    } else {
        *fn  = c;
        *fn1 = d;                         /* F(2k),   F(2k+1) */
    }
}

/* Return Fibonacci(n) modulo mod. */
static inline uint64_t fib_mod(uint64_t n, uint64_t mod)
{
    if (mod == 0) return 0;
    uint64_t fn, fn1;
    fib_pair(n, mod, &fn, &fn1);
    return fn;
}

/* Compute gcd of two unsigned 64‑bit integers. */
static inline uint64_t my_gcd(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* OEIS sequence A104714: gcd(Fibonacci(n), n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 by definition */

    uint64_t mod = (uint64_t)n;
    uint64_t f_mod = fib_mod((uint64_t)n, mod);   /* F(n) mod n */
    uint64_t g = my_gcd(mod, f_mod);             /* gcd(F(n), n) */
    return (seq_t)g;
}
