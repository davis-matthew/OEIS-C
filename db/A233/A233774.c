/*
 * A233774  Total number of vertices in the first n rows of Sierpinski gasket,
 *          with a(0) = 1.
 * https://oeis.org/A233774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + r(n), r(0)=1, r(n)=2*r(n-2^⌊log₂ n⌋) for n>0,
 *            with r(2^k)=2^k+1. (Thus a(0)=1, a(1)=3, a(2)=6, a(3)=10,
 *            a(4)=15, …)
 * Range:     u8 n <= 27, u16 n <= 932, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static caches */
static seq_t *r_cache = NULL;
static seq_t *a_cache = NULL;
static size_t  cache_cap  = 0;   /* current allocated size */
static idx_t   max_comp   = 0;   /* last computed index */
static bool    initialized = false;

/* index of most significant bit of v (v>0) */
static inline unsigned msb_index(idx_t v)
{
#if defined(__GNUC__) || defined(__clang__)
    /* 63 is set for 64‑bit idx_t; may be replaced by
       sizeof(idx_t)*8-1 if idx_t of other width is used   */
    return 63 - __builtin_clzll(v);
#else
    unsigned r = 0;
    while (v >>= 1) ++r;
    return r;
#endif
}

/* enlarge cache so that indices up to n are valid */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    r_cache = (seq_t *)realloc(r_cache, new_cap * sizeof(seq_t));
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* compute r(i) and a(i) for all i ≤ n */
static void compute_up_to(idx_t n)
{
    if (!initialized) {
        /* first time: initialise cache at i=0 */
        ensure_capacity(0);
        r_cache[0] = (seq_t)1;
        a_cache[0] = (seq_t)1;
        max_comp   = 0;
        initialized = true;
    }
    if (n <= max_comp) return;

    ensure_capacity(n);

    for (idx_t i = max_comp + 1; i <= n; ++i) {
        idx_t r_val;
        if (i == 0) {                 /* handled above, but kept for safety */
            r_val = 1;
        } else if ((i & (i - 1)) == 0) {   /* power of two */
            r_val = i + 1;
        } else {
            unsigned shift = msb_index(i);
            idx_t pow = (idx_t)1 << shift;
            r_val = 2 * (idx_t)r_cache[i - pow];
        }
        r_cache[i] = (seq_t)r_val;
        a_cache[i] = (seq_t)((idx_t)a_cache[i - 1] + r_val);
    }
    max_comp = n;
}

seq_t a(idx_t n)
{
    if (!initialized || n > max_comp) compute_up_to(n);
    return a_cache[n];
}
