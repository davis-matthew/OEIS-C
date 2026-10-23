/*
 * A028255  Fibonacci(n+3) mod n-th prime.
 * https://oeis.org/A028255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A028255:  a(n) = Fibonacci(n+3) mod p(n)
 *  where p(n) is the n-th prime.
 *  Implements fast‑doubling Fibonacci modulo a 64‑bit prime and
 *  caches the primes up to the requested index.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_mul.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/*  Compute F(n)  and  F(n+1)  modulo  mod.
 *  Results returned via the pointers *f and *f1.  */
static void fib_pair_mod(idx_t n, uint64_t mod, uint64_t *f, uint64_t *f1)
{
    if (n == 0) {
        *f  = 0;
        *f1 = 1;
        return;
    }

    uint64_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);          /* a = F(k), b = F(k+1)   */

    /*  Compute F(2k) and F(2k+1) using the doubling formulas.  */
    uint64_t t = ((2 * b) % mod + mod - a) % mod;          /* 2*b – a (mod) */
    uint64_t c = mod_mul(a, t, mod);                       /* F(2k)      */
    uint64_t d = (mod_mul(a, a, mod) + mod_mul(b, b, mod)) % mod; /* F(2k+1) */

    if (n & 1) {
        *f  = d;                       /* F(2k+1)    */
        *f1 = (c + d) % mod;           /* F(2k+2)    */
    } else {
        *f  = c;                       /* F(2k)      */
        *f1 = d;                       /* F(2k+1)    */
    }
}

/*  Return Fibonacci(n) modulo mod.  */
static uint64_t fib_mod_uint64(idx_t n, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t f, f1;
    fib_pair_mod(n, mod, &f, &f1);
    return f;
}

/*  Cached array of primes (1‑based).  */
static uint64_t *primes   = NULL;   /* points to p(1), p(2), ... */
static size_t   prime_cnt = 0;      /* number of cached primes    */

/*  OEIS sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    /*  Ensure at least n primes are cached.  */
    if ((size_t)n > prime_cnt) {
        idx_t limit = _prime_limit_for_idx(n);   /* upper bound for the n‑th prime */
        uint64_t *tmp = NULL;
        size_t   cnt  = 0;
        build_primes_upto(limit, &tmp, &cnt);
        /*  Free previous table and adopt the new one.  */
        free(primes);
        primes   = tmp;
        prime_cnt = cnt;
    }

    uint64_t prime_n = primes[n - 1];   /* n-th prime, 1‑based index */

    /*  Fibonacci index according to OEIS definition.  */
    idx_t fib_idx = n + 3;

    /*  Compute Fibonacci(n+3) mod p(n).  */
    uint64_t res = fib_mod_uint64(fib_idx, prime_n);

    /*  Cast result to the build’s seq_t type.  */
    return (seq_t)res;
}
