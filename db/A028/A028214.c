/*
 * A028214  Expansion of 1/((1-6x)(1-8x)(1-10x)(1-12x)).
 * https://oeis.org/A028214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*a(n-1) – 476*a(n-2) + 2736*a(n-3) – 5760*a(n-4) with
 *            initial values: a(0)=1, a(1)=36, a(2)=820, a(3)=15120
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, free, size_t */

extern seq_t a(idx_t n);

static seq_t *cache = NULL;     /* cache of computed terms */
static size_t cap = 0;          /* allocated size of cache */
static size_t computed_until = 0; /* largest index for which cache[i] is valid */

/* Ensure that the cache can hold at least indices up to 'need'. */
static void ensure_cache(idx_t need)
{
    size_t new_cap = cap;
    if (new_cap < 4)
        new_cap = 4;
    while (new_cap <= (size_t)need)
        new_cap *= 2;

    if (new_cap != cap) {
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* memory allocation failure – abort */
            abort();
        }
        /* Initialize new entries if necessary (only first 4 are needed). */
        if (cap == 0) {
            new_cache[0] = (seq_t)1;
            new_cache[1] = (seq_t)36;
            new_cache[2] = (seq_t)820;
            new_cache[3] = (seq_t)15120;
            computed_until = 3;
        } else {
            /* Copy existing data */
            for (size_t i = 0; i < cap; ++i)
                new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cap = new_cap;
    }
}

/* Compute a(n) for arbitrary n, memoising intermediate results. */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* safe for n up to 10000, fits in size_t */

    /* Initialise cache on first call */
    if (!cache) {
        ensure_cache(idx);           /* sets initial 4 values and computed_until */
    } else {
        if (idx > cap - 1)
            ensure_cache(idx);
    }

    /* Return already computed value if available */
    if (idx <= computed_until)
        return cache[idx];

    /* Compute missing terms up to idx */
    for (size_t i = computed_until + 1; i <= idx; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];

        cache[i] = (seq_t)36 * a1
                 - (seq_t)476 * a2
                 + (seq_t)2736 * a3
                 - (seq_t)5760 * a4;
    }
    computed_until = idx;
    return cache[idx];
}
