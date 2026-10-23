/*
 * A120501  Meta-Fibonacci sequence a(n) with parameters s=2.
 * https://oeis.org/A120501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for 1 ≤ n ≤ 3; a(4) = 2; for n > 4, a(n) =
 *            a(n-2-a(n-1)) + a(n-3-a(n-2))
 * Range:     u8 n <= 518, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* 1‑based array of computed terms */
static idx_t  cache_len = 0;   /* largest n for which cache[n] is valid (≥1) */
static idx_t  cache_cap = 0;   /* allocated capacity (maximum n that can be stored) */

static void
ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;   /* already large enough */

    /* Grow a bit beyond the required size */
    idx_t new_cap = need + 32;

#ifdef __cplusplus
    /* In C++ we can use new[] which constructs the objects */
    seq_t *new_cache = new seq_t[new_cap + 1];

    if (cache) {
        for (idx_t i = 1; i <= cache_len; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
    }
#else
    /* In C we use realloc; seq_t is expected to be trivially copyable */
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap + 1));
    if (!new_cache)
        abort();
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute the Meta‑Fibonacci term a(n).  n must be >= 1.  */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for offset 0 – return 0 */

    /* Initialise cache on first use.  All values are set only up to the
       required size. */
    if (cache_len == 0) {
        ensure_capacity(4);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache_len = 4;
    }

    /* Grow the cache if we need to compute farther terms. */
    if (n > cache_cap)
        ensure_capacity(n);

    /* Compute missing terms up to n. */
    while (cache_len < n) {
        idx_t i = cache_len + 1;

        /* a(i-1) and a(i-2) are already known because
           i > 4 ensures we are in the recurrence case.  */
        seq_t a_im1 = cache[i - 1];
        seq_t a_im2 = cache[i - 2];

        /* Sub‑indices are guaranteed to be >= 1 for i > 4.  */
        idx_t idx1 = (i - 2) - (idx_t)a_im1;
        idx_t idx2 = (i - 3) - (idx_t)a_im2;

        cache[i] = cache[idx1] + cache[idx2];
        cache_len = i;
    }

    return cache[n];
}
