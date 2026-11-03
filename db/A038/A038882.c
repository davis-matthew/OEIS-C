/*
 * A038882  Primes that are not in A038881.
 * https://oeis.org/A038882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p = 2 or jacobi_symbol(11,p) =
 *            -1 (i.e. 11 is a quadratic non‑residue modulo p)
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 954 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) = 2. */
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* Static cache of the sequence members. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* The next odd integer to test for primality (in uint64_t). */
    static uint64_t search_start = 3;

    /* Grow the cache until it contains at least n elements. */
    while (cache_size < (size_t)n)
    {
        /* The first term is the special split prime 2. */
        if (cache_size == 0)
        {
            /* Allocate initial capacity. */
            size_t new_cap = 16;
            cache = mem_resize(seq_t, cache, new_cap);
            if (!cache) exit(1);
            cache_cap = new_cap;
            cache[0] = (seq_t)2;
            cache_size = 1;
            continue;
        }

        /* Find next candidate prime. */
        uint64_t p = search_start;
        while (!is_prime((idx_t)p))
            p += 2;
        search_start = p + 2;   /* advance for the next call */

        /* Include the prime only if 11 is a quadratic non‑residue. */
        if (jacobi_symbol(11, p) == -1)
        {
            /* Grow cache capacity if necessary. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
