/*
 * A176610  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=0, k=1 and l=1.
 * https://oeis.org/A176610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 0 a(n) = Σ_{p=0}^{n‑1} a(p) * a(n‑1‑p) + n + 1
 *            for n ≥ 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A177610 – OEIS sequence A176610 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t capacity   = 0;   /* allocated size */
    static idx_t  computed   = 1;   /* highest index computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = (size_t)n + 1;
        if (capacity < 2) capacity = 2;          /* need at least a[0] and a[1] */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        computed = 1;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* return cached value if already known */
    if (n <= computed) return cache[n];

    /* compute new values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* convolution sum Σ_{p=0}^{i‑1} a(p) * a(i‑1‑p) */
        seq_t sum = (seq_t)0;
        idx_t limit = (i - 1) / 2;                /* last index where p <= q */
        for (idx_t p = 0; p <= limit; ++p) {
            idx_t q = (i - 1) - p;
            seq_t prod = cache[p] * cache[q];
            sum += prod;
            if (p != q) sum += prod;           /* pair counted twice */
        }
        /* add the linear term n+1 (here i+1, since i = n) */
        cache[i] = sum + (seq_t)i + (seq_t)1;
    }

    computed = n;
    return cache[n];
}
