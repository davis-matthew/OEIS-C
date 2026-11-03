/*
 * A142418  Primes congruent to 5 mod 49.
 * https://oeis.org/A142418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 5 (mod 49)
 * Range:     u8 n <= 2, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 948 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static dynamic array of the primes
   found so far.  For each call it extends the array until it
   contains at least n elements.  The sequence index n is 1‑based. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions provided by the project */
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
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

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* index 0 is undefined; return 0 */

    /* Static cache of found primes */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_cnt  = 0;
    static idx_t  last_k     = 0;  /* next candidate index (k)   */

    /* Extend the cache until it contains at least n primes */
    while (cache_cnt < n)
    {
        /* Candidate of the form 49*k + 5 */
        idx_t        k   = last_k;
        seq_t        cand = (seq_t)((idx_t)k * 49 + 5);

        /* If it is prime, store it in the cache */
        if (is_prime_big(cand))
        {
            if (cache_cnt == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = cand;
        }

        last_k = k + 1;   /* advance to next candidate */
    }

    return cache[n - 1];   /* n is 1‑based */
}
