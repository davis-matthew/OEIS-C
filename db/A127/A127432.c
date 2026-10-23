/*
 * A127432  a(0)=1. a(n) = number of earlier terms a(k), 0<=k<=n-1, such that
 *          (k+n) is divisible by a(k).
 * https://oeis.org/A127432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127432
   a(0) = 1
   a(n) = # { k : 0 ≤ k < n and (k + n) mod a(k) = 0 }
   The function memoises a(n) for 0 ≤ n in any order.
   Uses only 64‑bit unsigned arithmetic for the recurrence; the
   result is cast to seq_t, which may be any unsigned integer type
   (uint8_t … uint64_t, unsigned __int128, or an arbitrary‑precision
   integer).  This keeps the computation fast and avoids excessive
   memory usage from storing many big integer objects. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *cache_ul = NULL;  /* memoised terms as 64‑bit ints */
static size_t  cache_cap = 0;      /* allocated size */
static size_t  cache_len = 0;      /* number of values stored */

/* Ensure that cache_ul[i] is computed for all 0 ≤ i ≤ n */
static void ensure_cache(idx_t n)
{
    const size_t desired = (size_t)n + 1;
    if (desired <= cache_len)
        return;                      /* already ready */

    /* Grow capacity if necessary */
    if (cache_cap == 0) {
        cache_cap = 16;
        if (cache_cap < desired)
            cache_cap = desired;
    }
    while (cache_cap < desired) {
        cache_cap <<= 1;              /* double */
    }

    /* Reallocate array */
    uint64_t *new_ptr = (uint64_t *)realloc(cache_ul, cache_cap * sizeof(uint64_t));
    if (!new_ptr) {
        /* Out of memory; abort the program (as per build policy) */
        abort();
    }
    cache_ul = new_ptr;

    /* Compute new entries */
    for (size_t i = cache_len; i < desired; ++i) {
        if (i == 0) {                 /* a(0) = 1 */
            cache_ul[0] = 1ULL;
            continue;
        }

        uint64_t count = 0;
        uint64_t cur_n = (uint64_t)i;
        for (size_t k = 0; k < i; ++k) {
            uint64_t sum = (uint64_t)k + cur_n;
            if (sum % cache_ul[k] == 0)
                ++count;
        }
        cache_ul[i] = count;
    }
    cache_len = desired;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return (seq_t)cache_ul[(size_t)n];
}
