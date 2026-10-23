/*
 * A133438  a(1)=1. a(n) = Sum_{k|n} a(floor(sqrt(k))).
 * https://oeis.org/A133438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS: A133438
 * Recurrence: a(1) = 1
 * a(n) = Σ_{d|n} a( floor(√d) )  for n ≥ 2
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root, floor(√n) for 64‑bit unsigned values */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    for (int i = 62; i >= 0; --i) {
        idx_t b = r | ((idx_t)1 << i);
        if (n / b >= b)
            r = b;
    }
    return r;
}

/* cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cap   = 0;

/* make sure the cache can store indices up to 'needed' (inclusive) */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;

    size_t new_cap = cap ? cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* allocation failed */

    cache = tmp;
    /* initialise any newly added slots to 0 (uncomputed) */
    for (size_t i = cap; i < new_cap; ++i)
        cache[i] = 0;

    cap = new_cap;
}

/* compute a(n) using memoisation */
seq_t a(idx_t n)
{
    /* we use 1‑based indexing; index 0 is unused */
    ensure_capacity((size_t)n + 1);

    /* compute all missing terms up to n in ascending order */
    for (idx_t k = 1; k <= n; ++k) {
        if (cache[k] != 0 || k == 1)         /* 0 can be used only as a sentinel */
            continue;

        cache[1] = 1;                         /* base case */
        break;                                /* after first pass the loop will break */
    }

    /* full computed loop (safeguard for n==1) */
    for (idx_t k = 1; k <= n; ++k) {
        if (cache[k] != 0 && k != 1)
            continue;

        if (k == 1) {
            cache[1] = 1;
            continue;
        }

        seq_t sum = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t d1 = d;
                idx_t d2 = k / d;

                idx_t s1 = isqrt_u64(d1);
                seq_t val1 = cache[s1];
                sum += val1;

                if (d1 != d2) {
                    idx_t s2 = isqrt_u64(d2);
                    seq_t val2 = cache[s2];
                    sum += val2;
                }
            }
        }
        cache[k] = sum;
    }

    return cache[n];
}
