/*
 * A127580  Numbers k such that 64k+63 is prime.
 * https://oeis.org/A127580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127580: numbers k such that 64·k + 63 is prime */

/* The function a(n) returns the n‑th such k (1‑based).  It uses a
   simple cache so that repeated calls in any order are fast.  Primality
   is tested with the helper is_prime(idx_t), which works for values
   up to 2^64−1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache      = NULL;   /* stored k values       */
static size_t cache_cap  = 0;      /* allocated capacity    */
static idx_t cached_len  = 0;      /* number of k values    */
static idx_t next_k      = 1;      /* next candidate k      */

/* Grow the cache to hold at least 'needed' entries.  Uses seq_realloc
   to preserve correct construction of elements when seq_t is arbitrary
   precision. */
static void grow_cache(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();           /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term (1‑based).  The function is fully memoised and
   can be called in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, safe fallback */

    while (cached_len < n) {
        /* Test whether 64·next_k + 63 is prime. */
        idx_t candidate = 64 * next_k + 63;
        if (is_prime(candidate)) {
            grow_cache(cached_len + 1);
            cache[cached_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
