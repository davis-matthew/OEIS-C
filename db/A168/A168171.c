/*
 * A168171  Least prime p = 1 (mod n) which divides Fibonacci((p-1)/n).
 * https://oeis.org/A168171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least prime p ≡ 1 (mod n) such that p | Fib((p-1)/n)
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 982 (2026-11-03)
 * Helpers:   is_prime_u64
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
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}
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
