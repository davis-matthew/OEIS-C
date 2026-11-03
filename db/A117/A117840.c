/*
 * A117840  Smaller of two consecutive prime numbers with the same
 *          multiplicative digital root.
 * https://oeis.org/A117840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 4130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 8670 (2026-11-03)
 * Helpers:   digit_product, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A117840
 *
 * a(n) = the smaller of two consecutive primes whose multiplicative digital
 * root (product of digits repeatedly until a single digit) is equal.
 *
 * Implementation: iteratively generate primes using next_prime, compare
 * their multiplicative digital roots using digit_product, and cache found
 * a(n) values for future calls.
 */

#include <stddef.h>          /* size_t */
#include <stdint.h>          /* uint64_t */
#include <stdbool.h>
#include <stdlib.h>          /* malloc, abort, free */

#include "../utils/digit_product.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

static seq_t *
grow_cache(seq_t *cache, size_t *cap_ptr, size_t new_cap)
{
    seq_t *new_cache = mem_new(seq_t, new_cap);
    if (!new_cache) abort();
    if (cache) {
        for (size_t i = 0; i < *cap_ptr; ++i) new_cache[i] = cache[i];
        mem_free(cache);
    }
    *cap_ptr = new_cap;
    return new_cache;
}

/* multiplicative digital root, using digit_product and seq_cmp */
static seq_t
mdr(seq_t x)
{
    seq_t prod = digit_product(x);
    seq_t ten  = (seq_t)10;
    while (seq_cmp(&prod, &ten) >= 0) {
        prod = digit_product(prod);
    }
    return prod;
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* cached terms */
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static uint64_t curr     = 2;      /* prime to start from */
    static bool  initialized = false;

    if (!initialized) {
        cache_cap = 1024;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
        cache_len = 0;
        curr = 2;
        initialized = true;
    }

    /* generate terms until the cache holds at least n terms */
    while ((size_t)n > cache_len) {
        uint64_t p = curr;
        uint64_t q = next_prime(p + 1);

        seq_t root_p = mdr((seq_t)p);
        seq_t root_q = mdr((seq_t)q);

        if (seq_cmp(&root_p, &root_q) == 0) {
            if (cache_len == cache_cap) {
                cache = grow_cache(cache, &cache_cap, cache_cap * 2);
            }
            cache[cache_len++] = (seq_t)p;   /* store the smaller prime */
        }
        curr = q;   /* keep scanning consecutive primes */
    }

    return cache[n - 1];
}
