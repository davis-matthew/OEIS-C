/*
 * A142419  Primes congruent to 6 mod 49.
 * https://oeis.org/A142419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime congruent to 6 modulo 49
   (OEIS A142419).  It uses next_prime to iterate over primes and
   keeps a cache so that repeated calls with arbitrary ordering are
   efficient.  The cache is grown using operator new[] in C++ and
   malloc() in C, ensuring the code compiles in both languages. */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */
#include "../utils/next_prime.h"

/* Static cache of already found terms.  The cache is resized as needed.
   The implementation is independent of the concrete type of seq_t: in
   C++ we use new[]/delete[], in C malloc()/free(). */
static seq_t *cache = NULL;      /* array of terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static uint64_t last_prime = 0;  /* last prime examined */
static bool initialized = false;

/* Grow the cache to at least new_cap elements, copying existing data. */
static void grow_cache(size_t new_cap)
{
    if (cache_cap >= new_cap)
        return;
    /* Ensure power-of-two growth for simplicity */
    size_t cap = cache_cap ? cache_cap : 256;
    while (cap < new_cap)
        cap <<= 1;
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[cap];
#else
    seq_t *new_cache = (seq_t*)malloc(cap * sizeof(seq_t));
#endif
    if (cache) {
        /* copy old data */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
    }
    cache = new_cache;
    cache_cap = cap;
}

/* Ensure that the cache contains at least the first n terms. */
static void ensure_cache(idx_t n)
{
    if (!initialized) {
        grow_cache(256);
        initialized = true;
    }
    while (cache_len < n) {
        last_prime = next_prime(last_prime + 1);
        if ((last_prime % 49ULL) == 6ULL) {
            if (cache_len == cache_cap)
                grow_cache(cache_cap * 2);
            /* Implicit conversion from uint64_t to seq_t is used. */
            cache[cache_len++] = last_prime;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for n = 0; return 0 to avoid UB */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
