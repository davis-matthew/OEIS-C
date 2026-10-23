/*
 * A100622  Expansion of e.g.f. exp( (1+2*x-sqrt(1-4*x))/4).
 * https://oeis.org/A100622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4n-5)*a(n-1) - (4n-7)*a(n-2) + (n-2)*a(n-3) with a(0)=1,
 *            a(1)=1, a(2)=2 (valid for all n ≥ 0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses memoisation, computing terms on demand up to the
   requested index.  This works for 8‑, 16‑, 32‑, 64‑, 128‑bit and
   arbitrary‑precision seq_t types.  */
#include <stdlib.h>

/* External types provided by the build environment */
extern seq_t a(idx_t n);

/* static memoisation storage */
static seq_t *cache = NULL;   /* array of computed terms  */
static idx_t  cache_len = 0;  /* number of stored terms        */
static idx_t  cache_cap = 0;  /* allocated capacity            */

/* Ensure the cache can hold at least `need` terms.  If it already
   has enough space, nothing is done.  */
static void ensure_capacity(idx_t need) {
    if (need < cache_cap)
        return;
    /* double capacity until it covers need */
    idx_t new_cap = cache_cap == 0 ? 8 : cache_cap;
    while (new_cap <= need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* memory error – aborting is acceptable as the function is
           never called in a context that tolerates it.  */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term.  Calls will be in any order.  */
seq_t a(idx_t n) {
    /* Check if already computed.  */
    if (n < cache_len)
        return cache[n];

    /* Initialise table with initial values.  */
    if (cache_len == 0) {
        ensure_capacity(3);
        cache[0] = 1;      /* a(0) */
        cache[1] = 1;      /* a(1) */
        cache[2] = 2;      /* a(2) */
        cache_len = 3;
    }

    /* Grow cache up to n.  */
    if (n >= cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = 1;
            } else if (i == 1) {
                val = 1;
            } else if (i == 2) {
                val = 2;
            } else {
                /* recurrence: (4n-5)*a(n-1) - (4n-7)*a(n-2) + (n-2)*a(n-3) */
                seq_t m1 = (seq_t)(4 * i - 5) * cache[i - 1];
                seq_t m2 = (seq_t)(4 * i - 7) * cache[i - 2];
                seq_t m3 = (seq_t)(i - 2) * cache[i - 3];
                val = m1 - m2 + m3;
            }
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
