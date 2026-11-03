/*
 * A112044  Let b(0)=1/2, b(n) = (b(n-1)+Prime[n])/2; sequence gives
 *          2^(n+1)*b(n).
 * https://oeis.org/A112044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=a(n-1)+2^n * Prime[n]
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-03)
 * Helpers:   is_prime_trial, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_trial.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

/*--- dynamic array helpers for primes --------------------------------------*/
static void ensure_primes_cap(size_t *cap, uint64_t **arr, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t get_prime(uint64_t idx) {
    static uint64_t *prime_vec = NULL;
    static size_t prime_cap = 0;
    static size_t prime_sz  = 0;
    static uint64_t candidate = 3;      /* next odd to test */

    if (!prime_vec) { /* first call: initialise */
        ensure_primes_cap(&prime_cap, &prime_vec, 16);
        prime_vec[0] = 2;
        prime_sz = 1;
    }

    while (prime_sz < idx) {
        /* find next prime */
        while (!is_prime_trial(candidate, prime_vec, prime_sz))
            candidate += 2;

        /* add found prime */
        if (prime_sz == prime_cap)
            ensure_primes_cap(&prime_cap, &prime_vec, prime_cap << 1);

        prime_vec[prime_sz++] = candidate;
        candidate += 2;
    }
    return (seq_t)prime_vec[idx - 1];
}

/* Sequence computation: a(n) = 2^(n+1) * b(n) where
   b(0)=1/2, b(n)=(b(n-1)+Prime[n])/2.  */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* cache[0] = a(0) */
    static idx_t  cap    = 0;       /* allocated capacity */
    static idx_t  sz     = 0;       /* number of stored values */

    /* Initialise cache with a(0)=1 on first call */
    if (sz == 0) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        cache[0] = 1;   /* a(0) */
        sz = 1;
    }

    /* Grow cache if needed and compute up to n */
    while (sz <= n) {
        /* Compute a(sz) based on a(sz-1) */
        seq_t prev = cache[sz - 1];

        /* Prime[sz]  (Prime[1] = 2, Prime[2] = 3, ...) */
        seq_t p = get_prime(sz);

        /* 2^sz */
        seq_t two_pow = pow2(sz);

        /* a(sz) = prev + two_pow * p */
        seq_t add = two_pow * p;
        seq_t cur = prev + add;

        /* Ensure capacity */
        if (sz == cap) {
            cap <<= 1;  /* double */
            cache = mem_resize(seq_t, cache, cap);
        }

        cache[sz++] = cur;
    }

    return cache[n];
}
