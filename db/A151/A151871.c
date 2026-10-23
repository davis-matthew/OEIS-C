/*
 * A151871  a(0)=1; a(1)=0; a(2)=2; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=2*a(j) + 2*a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0)=1; a(1)=0; a(2)=2; * for n>=3 if n=2^i + j with 0<=j<2^i
 *            then * a(n)=2*a(j)+2*a(j+1) (add 1 if j==2^i-1)
 * Range:     u8 n <= 58, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: floor(log2(v)) for v>=1 */
static inline unsigned floor_log2_uint64(idx_t v) {
    unsigned r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Cache for computed values and a flag array */
static seq_t  *cache   = NULL;
static bool   *seen    = NULL;
static idx_t  cache_size = 0;

/* Grow the cache so that index n is addressable */
static void ensure_cache(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size) return;

    size_t new_cap = needed;
    seq_t  *c = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    bool   *s = (bool   *)realloc(seen,  new_cap * sizeof(bool));
    if (!c || !s) abort();                 /* memory failure */

    cache = c; seen = s;
    for (size_t i = cache_size; i < new_cap; ++i) seen[i] = false;
    cache_size = new_cap;
}

/* Recursive computation with memoisation */
static seq_t compute_val(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    ensure_cache(n);
    if (seen[n]) return cache[n];

    idx_t i    = floor_log2_uint64(n);
    idx_t pow2 = (idx_t)1 << i;        /* 2^i */
    idx_t j    = n - pow2;             /* 0 <= j < 2^i */

    seq_t a_j   = compute_val(j);
    seq_t a_j1  = compute_val(j + 1);
    seq_t res   = (seq_t)2 * a_j + (seq_t)2 * a_j1;
    if (j == pow2 - 1) res += (seq_t)1;

    cache[n] = res;
    seen[n]  = true;
    return res;
}

/* Public function requested by the build */
seq_t a(idx_t n) {
    return compute_val(n);
}
