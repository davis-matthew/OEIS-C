/*
 * A156324  a(1)=0, a(n+1) is smallest nonprime >= a(n)+n.
 * https://oeis.org/A156324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>=1, a(n+1) is the smallest nonprime >= a(n)+n
 * Range:     u8 n <= 22, u16 n <= 344, u32 n <= 448, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* allocated array of computed terms          */
static size_t cache_len = 0;     /* number of terms currently stored          */
static size_t cache_cap = 0;     /* allocated capacity (in terms)              */

/* Ensure the cache has space for at least 'needed' terms. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

#ifdef __cplusplus
    /* C++: use new/delete to honor constructors/destructors for arbitrary‑precision types. */
    seq_t *new_cache = new seq_t[new_cap];
    /* copy existing data */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
#else
    /* C: realloc is sufficient for plain integer types. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* out of memory */
    (void)cache; /* silence unused warning */
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, return 0 as a safe default */

    /* Extend cached values up to index n. */
    ensure_capacity(n);
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)0;
        } else {
            idx_t prev_index = i - 1;         /* previous term's index (1‑based) */
            seq_t prev_val   = cache[prev_index - 1];
            seq_t candidate  = prev_val + (seq_t)(prev_index);

            /* Find the smallest non‑prime >= candidate. */
            while (is_prime_big(candidate))
                ++candidate;

            cache[prev_index] = candidate;
        }
    }
    cache_len = n;
    return cache[n - 1];
}
