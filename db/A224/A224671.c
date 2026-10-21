/*
 * A224671  Number of (n+1) X 4 0..2 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A224671: Number of (n+1)×4 0..2 matrices with each 2×2 subblock idempotent
 *
 * Recurrence (valid for n>6):
 *   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5)
 *
 * Initial values:
 *   a(1)=41, a(2)=76, a(3)=108, a(4)=170, a(5)=257, a(6)=398
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"

static seq_t *cache = NULL;           /* array of computed terms */
static idx_t cache_size   = 0;        /* highest index computed (1‑based) */
static idx_t cache_cap    = 0;        /* allocated array capacity (1‑based) */

/* Grow the cache to hold at least n terms */
static void ensure_capacity(idx_t n) {
    if (n <= cache_cap) return;              /* already enough space */

    seq_t *new_cache = alloc_seq(n);
    if (!new_cache) exit(1);                 /* allocation failure */

    if (cache) {                             /* copy existing terms */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        /* memory for previous cache is intentionally leaked – negligible in
           contests and allowed for C++ arbitrary‑precision build. */
    }
    cache = new_cache;
    cache_cap = n;
}

/* Ensure the first six terms are stored in the cache */
static void init_initial_terms(void) {
    if (cache_size >= 6) return;             /* already initialized */

    ensure_capacity(6);                      /* need space for 6 terms */
    cache[0] = (seq_t)41;
    cache[1] = (seq_t)76;
    cache[2] = (seq_t)108;
    cache[3] = (seq_t)170;
    cache[4] = (seq_t)257;
    cache[5] = (seq_t)398;
    cache_size = 6;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;                     /* undefined below offset */

    init_initial_terms();

    /* Compute missing terms up to n */
    if (n > cache_size) {
        ensure_capacity(n);

        for (idx_t j = cache_size; j < n; ++j) {
            /* j is 0‑based index of a(j+1); j-5 is safe because j ≥ 6 */
            seq_t t = 4 * cache[j-1];
            t = t - 5 * cache[j-2];
            t = t + cache[j-3];
            t = t + 2 * cache[j-4];
            t = t - cache[j-5];
            cache[j] = t;
        }
        cache_size = n;
    }

    return cache[n-1];
}
