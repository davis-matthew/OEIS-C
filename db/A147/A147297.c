/*
 * A147297  Primes of the form (2k)^2 + 3(2k + 1)^2.
 * https://oeis.org/A147297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form (2k)^2 + 3(2k+1)^2, offset = 1
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm tests successive k = 1,2,3,…, keeps a dynamic cache of found primes. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* offset 1 – the first value corresponds to n = 1 */
    if (n == 0) return 0;          /* behaviour for n < 1 is irrelevant */

    static seq_t *cache   = NULL;  /* dynamic array of found primes */
    static idx_t  cache_len  = 0;  /* number of primes stored so far */
    static idx_t  cache_cap  = 0;  /* current capacity of the array */
    static idx_t  next_k     = 1;  /* k to be tested next */

    /* Extend cache until it contains at least n primes */
    while (cache_len < n)
    {
        seq_t k = (seq_t)next_k;
        /* (2k)^2 + 3(2k+1)^2  = 16k^2 + 12k + 3 */
        seq_t p = 16 * k * k + 12 * k + 3;

        if (is_prime_big(p))
        {
            /* grow cache if necessary */
            if (cache_len == cache_cap)
            {
                idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *tmp = seq_alloc((size_t)new_cap);
                if (cache != NULL)
                {
                    tmp = seq_realloc(cache, cache_cap, new_cap);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }
        ++next_k;
    }

    return cache[n - 1];
}
