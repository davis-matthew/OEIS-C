/*
 * A168171  Least prime p = 1 (mod n) which divides Fibonacci((p-1)/n).
 * https://oeis.org/A168171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least prime p ≡ 1 (mod n) such that p | Fib((p-1)/n)
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 52, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation is generic for the supported seq_t type.  It uses the
   arbitrary‑precision arithmetic provided by the build for seq_t and
   the big‑integer primality test `is_prime_big`.  The Fibonacci value is
   computed modulo p using the fast‑doubling method, all performed with
   seq_t arithmetic.  No external cache is used; each call is computed
   independently, which is fast enough for n ≤ 10 000. */

/* Include the required utilities. */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t) */
#include <stdint.h>                /* idx_t, seq_t are defined by the build */

/* Fast‑doubling Fibonacci modulo mod.
   Parameters:
       n   – Fibonacci index (idx_t)
       mod – modulus (seq_t)
       fn  – pointer to store F(n) mod mod
       fn1 – pointer to store F(n+1) mod mod */
static inline void fib_pair_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;   /* 1 mod mod, 0 if mod == 1 */
        return;
    }
    seq_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);   /* a = F(m), b = F(m+1) where m = n/2 */

    /* Reduce a and b modulo mod once; all further arithmetic is performed
       modulo mod to keep numbers small. */
    seq_t a_mod = a % mod;
    seq_t b_mod = b % mod;

    /* Compute c = F(2m)   = a*(2*b - a)   (mod mod) */
    seq_t two_b = (b_mod * 2) % mod;
    seq_t t = (two_b + mod - a_mod) % mod;           /* (2*b - a) mod mod */
    seq_t c = (a_mod * t) % mod;                    /* F(2m) */

    /* Compute d = F(2m+1) = a^2 + b^2   (mod mod) */
    seq_t a_sq = (a_mod * a_mod) % mod;
    seq_t b_sq = (b_mod * b_mod) % mod;
    seq_t d = (a_sq + b_sq) % mod;                  /* F(2m+1) */

    if ((n & 1) == 0) {    /* n even */
        *fn  = c;
        *fn1 = d;
    } else {                /* n odd */
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

static inline seq_t fib_mod(idx_t n, seq_t mod)
{
    seq_t fn, fn1;
    fib_pair_mod(n, mod, &fn, &fn1);
    return fn;
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0; return 0 */

    idx_t k = 1;            /* (p-1)/n */

    for (;;) {
        /* p = 1 + n*k, computed with seq_t arithmetic to avoid overflow */
        seq_t p = (seq_t)n * k + 1;

        /* Test primality of p */
        if (is_prime_big(p)) {
            /* Check if p | Fib(k) by evaluating Fib(k) mod p */
            if (fib_mod(k, p) == 0) {
                return p;
            }
        }
        ++k;
    }
}
