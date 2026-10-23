/*
 * A023174  Numbers k such that Fibonacci(k) == 2 (mod k).
 * https://oeis.org/A023174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that Fib(k) == 2 (mod k) */

#include <stdlib.h>
#include <stdint.h>

/* multiply (a*b) mod mod using 128‑bit intermediate to avoid overflow */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* fast doubling: computes (F(n), F(n+1)) mod mod */
static void fib_pair_mod_uint64(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) { *fn = 0; *fn1 = 1; return; }
    uint64_t a, b;
    fib_pair_mod_uint64(n >> 1, mod, &a, &b);

    uint64_t two_b = (uint64_t)(((__uint128_t)b * 2) % mod);
    uint64_t sub   = (two_b + mod - a) % mod;          /* (2*b - a) mod mod */
    uint64_t c     = mul_mod_uint64(a, sub, mod);      /* F(2k)   */
    uint64_t d     = (mul_mod_uint64(a, a, mod) +
                      mul_mod_uint64(b, b, mod)) % mod;/* F(2k+1) */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* Fibonacci(n) mod mod */
static uint64_t fib_mod_uint64(uint64_t n, uint64_t mod)
{
    uint64_t fn, fn1;
    fib_pair_mod_uint64(n, mod, &fn, &fn1);
    return fn;
}

/* Public interface required by the build system */
seq_t a(idx_t n)
{
    /* static state to allow calls in any order */
    static seq_t *list = NULL;    /* dynamic array of found k's          */
    static idx_t length = 0;      /* number of elements in list          */
    static idx_t capacity = 0;    /* allocated capacity of list          */
    static idx_t next_k = 1;      /* next integer k to test              */

    while (length < n) {
        uint64_t k = (uint64_t)next_k;
        uint64_t mod = k;
        uint64_t f = fib_mod_uint64(k, mod);
        uint64_t target = 2 % mod;
        if (f == target) {
            if (length == capacity) {
                size_t new_cap = capacity == 0 ? 8 : capacity * 2;
                seq_t *tmp = (seq_t*)realloc(list, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);               /* memory exhaustion */
                list = tmp;
                capacity = (idx_t)new_cap;
            }
            list[length++] = (seq_t)k;
        }
        next_k++;
    }

    return list[n-1];
}
