/*
 * A039819  Number of divisors of n-th refactorable number (A033950(n)).
 * https://oeis.org/A039819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of the n‑th refactorable number
 *            (A033950(n)). The value is obtained by scanning natural numbers,
 *            counting those for which τ(k) divides k, and taking τ of the
 *            requested one
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9868 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"

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

static seq_t *a_cache     = NULL;   /* cache of τ values for refactorables */
static size_t  cache_cap  = 0;      /* capacity of a_cache              */
static size_t  cache_size = 0;      /* number of computed values         */
static uint64_t last_checked = 1;   /* next integer to test             */

/* Factor count helper: returns divisor count of n (τ(n)) */
static inline seq_t tau(uint64_t n)
{
    return num_divisors(n);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range, safe fallback */

    /* Return cached value if already computed */
    if ((size_t)n <= cache_size)
        return a_cache[(size_t)n - 1];

    /* Ensure initial capacity */
    if (a_cache == NULL) {
        cache_cap = 64;
        a_cache = seq_alloc(cache_cap);
    }

    /* Scan integers until we reach the n‑th refactorable number */
    for (uint64_t i = last_checked; ; ++i) {
        seq_t t = tau(i);
        uint64_t t64 = (uint64_t)t;
        if (t64 != 0 && i % t64 == 0) {      /* i is refactorable */
            if (cache_size == cache_cap) {   /* grow cache */
                size_t new_cap = cache_cap * 2;
                a_cache = seq_realloc(a_cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            a_cache[cache_size++] = t;
            if ((size_t)n <= cache_size) {
                last_checked = i + 1;
                return a_cache[(size_t)n - 1];
            }
        }
    }
}
