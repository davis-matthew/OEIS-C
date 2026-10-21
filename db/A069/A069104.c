/*
 * A069104  Numbers m such that m divides Fibonacci(m+1).
 * https://oeis.org/A069104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code searches for numbers m such that m divides Fibonacci(m+1).
   It incrementally tests integers m and stores those that satisfy the
   divisibility condition.  Each call to a(n) returns the n‑th such number. */

#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* Multiply two 64‑bit numbers modulo mod, using 128‑bit arithmetic. */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}

/* Fast‑doubling algorithm for Fibonacci modulo mod.
   Returns F(n) mod mod.  n is 64‑bit, mod is positive and fits in 64 bits. */
static inline uint64_t fib_mod(uint64_t n, uint64_t mod)
{
    uint64_t a = 0;   /* F(k)   */
    uint64_t b = 1;   /* F(k+1) */
    if (n == 0) return 0;

    int msb = 63 - __builtin_clzll(n);
    for (int i = msb; i >= 0; --i) {
        /* Compute F(2k) and F(2k+1) from (a,b) = (F(k),F(k+1)) */
        uint64_t c = mul_mod(a, (mul_mod(2, b, mod) + mod - a) % mod, mod);      /* F(2k)   */
        uint64_t d = (mul_mod(a, a, mod) + mul_mod(b, b, mod)) % mod;            /* F(2k+1) */
        if ((n >> i) & 1) {
            a = d;
            b = (c + d) % mod;
        } else {
            a = c;
            b = d;
        }
    }
    return a;  /* F(n) mod mod */
}

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++: use a vector to hold the cached terms. */
    static std::vector<seq_t> cache;
    static uint64_t next = 1;          /* next m to test */
    while (cache.size() < n) {
        uint64_t m = next++;
        if (fib_mod(m + 1, m) == 0) {
            cache.push_back(seq_t(m));
        }
    }
    return cache[n - 1];
#else
    /* C: use a malloc/realloc array.  Assumes seq_t is a trivial type
       (e.g., unsigned long long).  The build must provide a suitable
       seq_t for C. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static uint64_t next = 1;          /* next m to test */
    while (cache_size < n) {
        uint64_t m = next++;
        if (fib_mod(m + 1, m) == 0) {
            seq_t *tmp = realloc(cache,
                                 (cache_size + 1) * sizeof(seq_t));
            if (!tmp) abort();  /* out of memory */
            cache = tmp;
            cache[cache_size++] = (seq_t)m;
        }
    }
    return cache[n - 1];
#endif
}
