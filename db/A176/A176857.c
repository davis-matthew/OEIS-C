/*
 * A176857  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=3, k=-1 and l=0.
 * https://oeis.org/A176857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1)=sum_{p=0..n} (a(p)*a(n-p)+k) with k=-1, l=0. Recurrence
 *            form: a(0)=1, a(1)=3, a(n+1)=sum_{p=0}^{n} a(p)*a(n-p) - (n+1)
 *            for n>=1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Memoized cache for sequence values */
static seq_t *seq_cache = NULL;
static size_t  cache_cap = 0;
static idx_t  cache_len = 0;   /* last computed index +1 */

/* Ensure cache_p has space for indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n + 1) {
        size_t new_cap = (cache_cap == 0 ? 4 : cache_cap * 2);
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        seq_cache = tmp;
        cache_cap  = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Initialize base values lazily */
    if (cache_len <= 0) {
        seq_cache[0] = 1;
        cache_len = 1;
    }
    if (cache_len <= 1 && n >= 1) {
        seq_cache[1] = 3;
        cache_len = 2;
    }

    /* Compute new terms up to n */
    for (idx_t cur = cache_len; cur <= n; ++cur)
    {
        if (cur == 0) continue;          /* already set */
        if (cur == 1) continue;          /* already set */

        /* Compute a[cur] using a[cur-1] as the "n" in the recurrence */
        seq_t sum = 0;
        /* sum_{p=0}^{cur-1} a[p] * a[cur-1-p] */
        for (idx_t p = 0; p <= cur - 1; ++p) {
            sum += seq_cache[p] * seq_cache[cur - 1 - p];
        }
        /* subtract (cur) because n+1 = cur in the formula */
        seq_cache[cur] = sum - (seq_t)cur;

        /* Update length */
        cache_len = cur + 1;
    }

    return seq_cache[n];
}
