/*
 * A177904  a(1)=a(2)=a(3)=1; thereafter a(n) = gpf(a(n-1)+a(n-2)+a(n-3)),
 *          where gpf = "greatest prime factor".
 * https://oeis.org/A177904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=a(2)=a(3)=1; a(n)=gpf(a(n-1)+a(n-2)+a(n-3)) for n>3
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gpf.h"

static seq_t *cache = NULL;    /* cache[i] holds a(i+1) */
static size_t cache_cap = 0;   /* current number of stored terms */

/* Return the n‑th term of OEIS A177904 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n < 1; return 0 for safety */
        return (seq_t)0;

    size_t idx = (size_t)n;

    /* If already computed, return it. */
    if (idx <= cache_cap)
        return cache[idx - 1];

    /* Expand the cache to store up to n terms. */
    size_t new_cap = idx;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();                   /* out of memory */

    cache = new_ptr;
    size_t old_cap = cache_cap;   /* previous number of stored terms */
    cache_cap = new_cap;

    /* Compute the newly needed terms. */
    for (size_t i = old_cap; i < cache_cap; ++i)
    {
        if (i < 3) {                   /* a(1), a(2), a(3) */
            cache[i] = (seq_t)1;
        } else {
            seq_t sum = cache[i - 1] + cache[i - 2] + cache[i - 3];
            /* The sum fits in uint64_t for the range of n we handle. */
            cache[i] = (seq_t)gpf((uint64_t)sum);
        }
    }

    return cache[idx - 1];
}
