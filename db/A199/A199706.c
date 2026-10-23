/*
 * A199706  Number of -n..n arrays x(0..3) of 4 elements with zero sum and no
 *          two neighbors equal.
 * https://oeis.org/A199706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)-3*a(n-2)+2*a(n-3)-3*a(n-4)+3*a(n-5)-a(n-6)
 *            Offset: 1 (a(1) = 8, a(2) = 52, a(3) = 168, a(4) = 380, a(5) =
 *            724, a(6) = 1236)
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[6] = {
    (seq_t)8,    /* a(1) */
    (seq_t)52,   /* a(2) */
    (seq_t)168,  /* a(3) */
    (seq_t)380,  /* a(4) */
    (seq_t)724,  /* a(5) */
    (seq_t)1236  /* a(6) */
};

static seq_t *cache       = NULL; /* dynamic cache of computed terms */
static size_t cached_cap  = 0;     /* allocated capacity (in terms) */
static idx_t  cached_max  = 0;     /* highest n for which a(n) has been cached */

/* Returns the n-th term of OEIS A199706 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset is 1; undefined for n = 0 */

    /* Ensure cache has room for n terms. */
    if (cached_cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                /* memory allocation failed */
        cache     = tmp;
        cached_cap = new_cap;
    }

    /* Copy the first six terms into the cache if not already done. */
    if (cached_max < 6) {
        size_t start = cached_max;                     /* 0‑based index */
        size_t end   = n < 6 ? n : 6;                  /* up to 6 terms */
        for (size_t i = start; i < end; ++i)
            cache[i] = init_vals[i];
        if (end > cached_max)
            cached_max = end;
    }

    /* If the requested term is already cached, return it. */
    if (n <= cached_max)
        return cache[n - 1];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 6) {                     /* copy initial values */
            cache[i - 1] = init_vals[i - 1];
        } else {
            /* Recurrence: a(i) = 3*a(i-1)-3*a(i-2)+2*a(i-3)-3*a(i-4)+3*a(i-5)-a(i-6) */
            seq_t t1 = cache[i - 2];
            t1 *= (seq_t)3;
            seq_t t2 = cache[i - 3];
            t2 *= (seq_t)3;
            seq_t sum = t1 - t2;          /* term 3*a(i-1) - 3*a(i-2) */

            seq_t t3 = cache[i - 4];
            t3 *= (seq_t)2;
            seq_t t4 = cache[i - 5];
            t4 *= (seq_t)3;
            sum += t3 - t4;              /* + 2*a(i-3) - 3*a(i-4) */

            seq_t t5 = cache[i - 6];
            t5 *= (seq_t)3;
            seq_t t6 = cache[i - 7];
            sum += t5 - t6;              /* + 3*a(i-5) - a(i-6) */

            cache[i - 1] = sum;
        }
    }
    cached_max = n;
    return cache[n - 1];
}
