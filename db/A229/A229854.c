/*
 * A229854  Primes of the form 384*k + 1.
 * https://oeis.org/A229854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime of the form 384*k + 1.
   The implementation keeps a growing cache of found primes so that
   calls with arbitrary n (not necessarily monotone) are efficient. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
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

/* Realloc helper that works in both C and C++.
   In C++ we use static_cast, in C we use a C-style cast. */
#if defined(__cplusplus)
    #define REALLOC(ptr, sz) (mem_resize(seq_t, (ptr), ((sz)) / sizeof(seq_t)))
#else
    #define REALLOC(ptr, sz) (mem_resize(seq_t, (ptr), ((sz)) / sizeof(seq_t)))
#endif

/* Static cache of found primes, indexed from 0. */
static seq_t *cache      = NULL;   /* array of found primes   */
static size_t cache_size = 0;      /* number of primes cached */
static size_t cache_cap  = 0;      /* allocated capacity      */
/* Next k to test (candidate p = 384*k + 1).  We start at k = 2 because
   384*1+1 = 385 is not prime. */
static idx_t next_k = 2;

/* Return the n‑th prime of the form 384*k + 1 (a(1) is the first). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1 */

    /* Grow the cache until it contains at least n primes. */
    while ((idx_t)cache_size < n)
    {
        /* Compute the candidate prime = 384*next_k + 1 using 64‑bit arithmetic. */
        seq_t cand = (seq_t)(((uint64_t)384 * next_k) + 1ULL);

        /* Test primality of cand. */
        if (is_prime_big(cand))
        {
            /* Ensure capacity of the cache. */
            if (cache_size == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = REALLOC(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;   /* allocation failed */
                cache = tmp;
            }
            /* Store the prime. */
            cache[cache_size++] = cand;
        }

        ++next_k;   /* next candidate */
    }

    /* Return the n-th prime (1‑based index). */
    return cache[n - 1];
}
