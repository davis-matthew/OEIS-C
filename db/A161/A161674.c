/*
 * A161674  Positions n such that A010060(n) + A010060(n+2) = 1.
 * https://oeis.org/A161674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the integer n (0‑based) for which Thue‑Morse(n) !=
 *            Thue‑Morse(n+2), i.e. A010060(n)+A010060(n+2)=1 (offset 1). The
 *            function builds the sequence on demand and memoises the values
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* parity of popcount of a 64‑bit integer (Thue‑Morse value) */
static inline bool parity64(uint64_t v)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_parityll(v);
#else
    v ^= v >> 32;
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    return v & 1;
#endif
}

/* Cached sequence values, zero‑based positions */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Next integer to test for the condition */
static idx_t next_test_pos = 0;

/* enlarge the cache array */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp) /* fatal error – abort */
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* extend the cache until it contains at least `target` elements */
static void extend_cache_until(idx_t target)
{
    while (cache_len < target) {
        bool p1 = parity64((uint64_t)next_test_pos);
        bool p2 = parity64((uint64_t)(next_test_pos + 2ULL));
        if (p1 != p2) { /* Thue‑Morse differs at n and n+2 */
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)next_test_pos;
        }
        ++next_test_pos;
    }
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; safe guard */
    extend_cache_until(n);
    return cache[n - 1];
}
