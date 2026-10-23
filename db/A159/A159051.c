/*
 * A159051  Numbers n such that Fibonacci(n-2) is divisible by n.
 * https://oeis.org/A159051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 894, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159051: a(n) is the n-th positive integer k such that
   Fibonacci(k−2) is divisible by k.  The function searches the
   integers in order, computing Fibonacci numbers modulo k via fast
   doubling, and memoises the results so that repeated calls are
   inexpensive. */

#include <stdint.h>
#include <stdlib.h>

/* Helper: compute F_n mod m using fast doubling, with all
   intermediate calculations performed modulo m.  The result
   fits in 0 … m−1 and is cast to seq_t before being returned. */
static inline seq_t fib_mod_u64(idx_t n, idx_t m)
{
    if (m == 1) return 0;   /* everything is 0 mod 1 */

    /* run the fast‑doubling algorithm iteratively */
    uint64_t a = 0;   /* F(0) */
    uint64_t b = 1;   /* F(1) */
    uint64_t mod = (uint64_t)m;

    /* determine most significant bit of n */
    int msb = -1;
    uint64_t t = n;
    while (t) {
        msb++;
        t >>= 1;
    }

    for (int i = msb; i >= 0; --i) {
        /* doubling step */
        unsigned __int128 aa = a;
        unsigned __int128 bb = b;
        unsigned __int128 two_b = bb * 2;
        uint64_t two_b_mod = (uint64_t)(two_b % mod);
        uint64_t diff_mod = (two_b_mod + mod - a) % mod;
        uint64_t c = (uint64_t)((aa * diff_mod) % mod);
        uint64_t d = (uint64_t)(((aa * aa) + (bb * bb)) % mod);

        if ((n >> i) & 1) {
            a = d;
            b = (c + d) % mod;
        } else {
            a = c;
            b = d;
        }
    }
    return (seq_t)a;   /* F_n mod m */
}

/* Main function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    /* static state across calls */
    static seq_t *cache = NULL;
    static size_t count = 0;   /* number of terms found */
    static size_t cap   = 0;   /* allocated capacity of cache */
    static idx_t current = 2;  /* next integer to test */

    /* grow the cache if needed */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend the cache until we have at least n terms */
    while (count < n) {
        /* test current candidate */
        if (fib_mod_u64(current - 2, current) == 0) {
            if (count >= cap) {
                cap = cap * 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[count++] = (seq_t)current;
        }
        ++current;
    }

    return cache[n - 1];
}
