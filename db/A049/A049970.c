/*
 * A049970  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 2.
 * https://oeis.org/A049970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049970
   a(1) = 1, a(2) = 3, a(3) = 2.
   For n ≥ 4:
        p  = floor(log2(n‑2))
        m  = n‑1‑(1 << p)
        a(n) = Σ_{k=1}^{n‑1} a(k) + a(m)
   The implementation memoises all computed values and prefix sums,
   allowing calls in any order.  It works with 8/16/32/64‑bit
   unsigned types and with arbitrary‑precision seq_t classes. */

#include <stdlib.h>

/* Cached sequence values and cumulative sums.  cache_len is the highest
   index that has already been computed (inclusive). */
static seq_t *cache_a   = NULL;   /* a[i]  (1‑based) */
static seq_t *cache_ps  = NULL;   /* ps[i] = Σ_{k=1}^i a(k) */
static idx_t  cache_len = 0;
static size_t cache_cap = 0;

/* floor_log2 for uint64_t: largest p with (1ULL<<p) <= n.
   n>0 by caller. */
static idx_t floor_log2_u64(idx_t n)
{
    idx_t r = 0;
    while (n >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, safe value */

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;

        seq_t *tmp_a  = (seq_t*)realloc(cache_a,  sizeof(seq_t) * (new_cap + 1));
        seq_t *tmp_ps = (seq_t*)realloc(cache_ps, sizeof(seq_t) * (new_cap + 1));
        if (!tmp_a || !tmp_ps) return 0;   /* allocation failed */

        cache_a   = tmp_a;
        cache_ps  = tmp_ps;
        cache_cap = new_cap;
    }

    /* If n is already computed, return it. */
    if (n <= cache_len) return cache_a[n];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache_a[1] = (seq_t)1;
            cache_ps[1] = cache_a[1];
        } else if (i == 2) {
            cache_a[2] = (seq_t)3;
            cache_ps[2] = cache_ps[1] + cache_a[2];
        } else if (i == 3) {
            cache_a[3] = (seq_t)2;
            cache_ps[3] = cache_ps[2] + cache_a[3];
        } else {
            /* p = floor(log2(i-2))   (note: i-2 > 0 for i≥4) */
            idx_t p = floor_log2_u64(i - 2);
            /* 2^p as a seq_t value.  For built‑in integer types
               the shift is performed on an integer literal converted
               to seq_t.  For big integer types, the conversion
               constructor and operator<< are assumed to exist. */
            seq_t pow2p = (seq_t)1 << (unsigned int)p;
            idx_t m = (i - 1) - (idx_t)pow2p;   /* m ≥ 1 for i≥4 */
            cache_a[i] = cache_ps[i - 1] + cache_a[m];
            cache_ps[i] = cache_ps[i - 1] + cache_a[i];
        }
    }

    cache_len = n;
    return cache_a[n];
}
