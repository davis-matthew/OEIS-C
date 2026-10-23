/*
 * A023177  Numbers k such that Fibonacci(k) == 8 (mod k).
 * https://oeis.org/A023177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that Fibonacci(k) ≡ 8
 *            (mod k)
 * Range:     u8 n <= 6, u16 n <= 349, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Return (a*b) mod mod using 128‑bit arithmetic to avoid overflow. */
static uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}

/* Fast‑doubling Fibonacci modulo m.
   f  = F(n)   (mod m)
   f1 = F(n+1) (mod m) *///(m may be 1; %1 yields 0)
static void fib_pair_mod(uint64_t n, uint64_t m, uint64_t *f, uint64_t *f1)
{
    if (n == 0) {
        *f  = 0;
        *f1 = 1 % m;           /* F(1) = 1, but we need it modulo m */
        return;
    }
    uint64_t a, b;
    fib_pair_mod(n >> 1, m, &a, &b);

    /* c = F(2k) = F(k) * (2*F(k+1) – F(k)) */
    uint64_t t = (2 * b) % m;
    t = (t + m - a) % m;
    uint64_t c = mulmod64(a, t, m);

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    uint64_t d = (mulmod64(a, a, m) + mulmod64(b, b, m)) % m;

    if ((n & 1) == 0) {
        *f  = c;
        *f1 = d;
    } else {
        *f  = d;
        *f1 = (c + d) % m;
    }
}

/* Compute Fibonacci(k) modulo k. */
static uint64_t fib_mod_k(uint64_t k)
{
    uint64_t f, f1;
    fib_pair_mod(k, k, &f, &f1);
    return f;
}

/* Return the n-th term of A023177.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic array of found k values */
    static size_t cap   = 0;      /* allocated capacity (in seq_t units) */
    static size_t len   = 0;      /* number of terms currently stored */
    static uint64_t last_k = 0;   /* last k examined */

    while (len < n) {
        ++last_k;
        uint64_t fk = fib_mod_k(last_k);
        uint64_t need = 8 % last_k;     /* 8 % last_k handles all k */
        if (fk == need) {
            if (len >= cap) {           /* grow the buffer */
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = (seq_t)last_k;
        }
    }
    return cache[n - 1];
}
