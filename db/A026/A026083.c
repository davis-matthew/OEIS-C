/*
 * A026083  a(n) = number of (s(0), s(1), ..., s(n)) such that every s(i) is
 *          an integer, s(0) = 0 = s(n), |s(i) - s(i-1)| = 1 for i = 1,2,3;
 *          |s(i) - s(i-1)| <= 1 for i >= 4. Also a(n) = T(n,n), where T is
 *          the array defined in A026082.
 * https://oeis.org/A026083
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   n*a(n) + (-3n+5)*a(n-1) + (-n-6)*a(n-2) + 3*(n-5)*a(n-3) = 0
 *            Initial values: a(4)=6, a(5)=12, a(6)=38
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed values */
static idx_t cache_size = 0;         /* size of allocated cache */

static void ensure_cache(idx_t n)
{
    /* make sure cache has entries up to n inclusive */
    if (cache_size <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; for the purposes of this exercise,
               we simply exit, though a proper implementation should
               handle this case gracefully. */
            exit(1);
        }
        /* zero-initialize newly added entries */
        idx_t i = cache_size;
        for (; i <= n; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;     /* undefined for n < 4 */

    /* initialize cache up to required n */
    ensure_cache(n);

    /* bottom‑up compute missing values */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;  /* already computed */

        if (i == 4) { cache[4] = (seq_t)6; continue; }
        if (i == 5) { cache[5] = (seq_t)12; continue; }
        if (i == 6) { cache[6] = (seq_t)38; continue; }

        /* recurrence: a(i) = ((3*i-5)*a(i-1) + (i+6)*a(i-2) - 3*(i-5)*a(i-3)) / i */
        seq_t term1 = (seq_t)(3 * i - 5) * cache[i - 1];
        seq_t term2 = (seq_t)(i + 6) * cache[i - 2];
        seq_t term3 = (seq_t)3 * (i - 5) * cache[i - 3];
        seq_t numerator = term1 + term2 - term3;
        cache[i] = numerator / (seq_t)i;  /* division is exact */
    }
    return cache[n];
}
