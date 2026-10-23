/*
 * A174562  a(1)=2, a(2)=3, then a(n)=a(n-1)+a(n-2) if n odd,
 *          a(n)=a(n-1)-a(n-2) if n even.
 * https://oeis.org/A174562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2, a(2) = 3; For n ≥ 3: a(n) = a(n-1) + a(n-2) if n is
 *            odd, a(n) = a(n-1) – a(n-2) if n is even. This recurrence can
 *            also be written as a(n) = a(n-2)+a(n-4)
 * Range:     u8 n <= 20, u16 n <= 42, u32 n <= 88, u64 n <= 182,
 *            u128 n <= 366, big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range, undefined */
    /* Cached values: cache[i] holds a(i+1) (0‑based). */
    static seq_t *cache      = NULL;     /* pointer to array of seq_t */
    static idx_t  cache_len  = 0;        /* number of terms already computed */
    static idx_t  cache_cap  = 0;        /* capacity of the allocated array */

    /* Grow cache if necessary. */
    if (cache_len < n) {
        /* Ensure capacity at least max(n, 2) and at least 2 for base cases. */
        idx_t need = n;
        if (need < 2) need = 2;
        if (cache_cap < need) {
            idx_t new_cap = cache_cap == 0 ? 2 : cache_cap * 2;
            if (new_cap < need) new_cap = need;
            seq_t *new_cache = seq_alloc_array(new_cap);
            if (cache) {
                for (idx_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                free_seq_array(cache);
            }
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Initialise first two terms if this is the first call. */
        if (cache_len == 0) {
            cache[0] = (seq_t)2;
            cache[1] = (seq_t)3;
            cache_len = 2;               /* we have computed up to a(2) */
        }

        /* Extend computation up to n. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i & 1) {                 /* odd */
                cache[i - 1] = cache[i - 2] + cache[i - 3];
            } else {                       /* even */
                cache[i - 1] = cache[i - 2] - cache[i - 3];
            }
        }
        if (n > cache_len) cache_len = n;  /* keep the largest computed index */
    }

    return cache[n - 1];   /* cache is 0‑based, n is 1‑based */
}
