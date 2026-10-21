/*
 * A050100  a(n) = floor(a(n-1)/3) if this is positive and not yet in the
 *          sequence, otherwise a(n) = 10*a(n-1).
 * https://oeis.org/A050100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A050100. Recurrence: a(1)=1;
   for n>1, let cand = floor(a(n-1)/3).
   If cand>0 and not already in the sequence, a(n)=cand;
   else a(n)=10*a(n-1). */

/* The function caches computed values so calls in any order are fast. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"

static seq_t *cache = NULL;   /* dynamically grown array of computed terms */
static size_t cache_len = 0;  /* number of terms currently stored */
static size_t cache_cap = 0;  /* current allocation capacity */

/* Allocate or grow the cache to at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;                 /* already big enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;          /* double until large enough */

    seq_t *new_ptr = seq_alloc(new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        new_ptr[i] = cache[i];   /* copy old data */

    cache = new_ptr;            /* replace old buffer, leak is harmless */
    cache_cap = new_cap;
}

/* Return the n-th term (1‑based) of A050100. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined index, return 0 for safety */

    size_t target = (size_t)n;
    grow_cache(target);

    /* Initialize cache with first term if empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
    }

    /* Compute terms up to the requested index. */
    while (cache_len < target) {
        seq_t prev = cache[cache_len - 1];
        seq_t cand = prev / (seq_t)3;

        bool used = false;
        if (cand > (seq_t)0) {
            /* Linear search for cand in the already computed part. */
            for (size_t i = 0; i < cache_len; ++i) {
                if (cache[i] == cand) {
                    used = true;
                    break;
                }
            }
        } else {
            used = true;           /* candidate not positive */
        }

        cache[cache_len] = used ? prev * (seq_t)10 : cand;
        ++cache_len;
    }

    return cache[target - 1];
}
