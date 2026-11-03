/*
 * A204827  Deficient numbers with even sum of divisors.
 * https://oeis.org/A204827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9133 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204827: deficient numbers with even sum of divisors.
   The sequence is generated on demand and cached.
   a(1) = 3, a(2) = 5, … */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
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

/* Cached terms and state */
static seq_t *cache = NULL;      /* array of terms */
static size_t cache_cap = 0;     /* current capacity */
static size_t cache_len = 0;     /* number of terms stored */
static uint64_t next_candidate = 2;  /* next integer to test */

/* Ensure the cache can hold at least `new_cap` terms. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    size_t cap = cache_cap ? cache_cap * 2 : 16;
    while (cap < new_cap)
        cap *= 2;

    if (cache == NULL) {
        cache = seq_alloc(cap);
    } else {
        cache = seq_realloc(cache, cache_cap, cap);
    }
    cache_cap = cap;
}

/* Return the n‑th term of A204827 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* Undefined for n==0; return 0 as a safe fallback. */

    size_t target = (size_t)n;   /* target index in the cache (0‑based) */

    while (cache_len < target) {
        uint64_t m = next_candidate++;
        seq_t sigma = sigma_of((idx_t)m);
        seq_t two_m = (seq_t)m * 2;

        /* Deficient: sigma(m) < 2*m  AND  sigma(m) even */
        if (sigma < two_m && ((sigma & 1) == 0)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[target - 1];
}
