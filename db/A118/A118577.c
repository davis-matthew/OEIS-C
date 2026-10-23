/*
 * A118577  a(1) = 1. For m >= 0 and 1 <= k <= 2^m, a(2^m +k) = number of
 *          earlier terms of the sequence which are coprime to a(k).
 * https://oeis.org/A118577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1.
   For m ≥ 0 and 1 ≤ k ≤ 2^m,
     a(2^m + k) = |{ j : 1 ≤ j < 2^m + k  ∧  gcd(a(j), a(k)) = 1 }|   */
#include <stdlib.h>
#include "../utils/gcd.h"          /* extern seq_t gcd(seq_t a, seq_t b); */

static seq_t *cache = NULL;       /* 1‑based sequence, stored 0‑based */
static size_t cache_size = 0;     /* number of terms already computed */
static size_t cache_cap   = 0;     /* capacity of the cache array */

/* Ensure the cache has space for at least <needed> elements. */
static void ensure_capacity(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* floor(log2(v)) for non‑zero v.  v is uint64_t. */
static unsigned floor_log2_uint64(idx_t v) {
    unsigned pos = 0;
    while (v >>= 1)
        pos++;
    return pos;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe return */

    /* Initialise with a(1) if the cache is empty. */
    if (cache_size == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Compute terms up to n if necessary. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* Determine m such that 2^m ≤ i-1 < 2^(m+1). */
        unsigned m = floor_log2_uint64(i - 1);
        idx_t pow2 = ((idx_t)1) << m;
        idx_t k = i - pow2;              /* 1 ≤ k ≤ 2^m */

        seq_t ref_val = cache[k - 1];    /* a(k) */

        seq_t cnt = 0;
        for (idx_t j = 0; j < i - 1; ++j) {
            if (gcd(cache[j], ref_val) == (seq_t)1)
                ++cnt;
        }

        ensure_capacity(i);
        cache[i - 1] = cnt;
        cache_size = i;
    }

    return cache[n - 1];
}
