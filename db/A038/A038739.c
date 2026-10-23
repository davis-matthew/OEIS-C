/*
 * A038739  T(n,n-2), array T as in A038738.
 * https://oeis.org/A038739
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(n, n‑2) – six‑diagonal of array defined in A038738.
 * The sequence satisfies the linear recurrence (for n ≥ 8):
 *   a(n) = 7*a(n‑1) - 19*a(n‑2) + 26*a(n‑3)
 *          - 19*a(n‑4) + 7*a(n‑5) - a(n‑6),
 * with initial values
 *   a(2)=1, a(3)=7, a(4)=30, a(5)=103, a(6)=314, a(7)=895.
 */
#include <stdlib.h>

/* Pre‑computed values for indices 0..7 (0 and 1 unused). */
static const seq_t INIT[8] = {
    0,          /* a(0) – unused */
    0,          /* a(1) – unused */
    1,          /* a(2) */
    7,          /* a(3) */
    30,         /* a(4) */
    103,        /* a(5) */
    314,        /* a(6) */
    895         /* a(7) */
};

/* Cached values: cache[i] holds a(i) for i ≤ cache_size‑1.   */
static seq_t *cache = NULL;          /* NULL until first use */
static size_t cache_size = 0;        /* number of allocated entries   */
static size_t high_computed = 7;     /* highest index whose value is in the cache */

/* Ensure that cache has space for at least `need` entries (0‑based).  */
static void grow_cache(size_t need)
{
    if (need <= cache_size)
        return;

    size_t new_size = cache_size ? cache_size * 2 : 16;
    if (new_size < need)
        new_size = need;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {           /* allocation failed – propagate zeroes */
        return;
    }
    cache = new_cache;
    /* initialise newly allocated slots to zero */
    for (size_t i = cache_size; i < new_size; ++i)
        cache[i] = (seq_t)0;
    cache_size = new_size;
}

/* The sequence function */
seq_t a(idx_t n)
{
    if (n < 2) /* undefined below offset */
        return (seq_t)0;

    /* For the first few terms use the table */
    if (n <= 7)
        return INIT[n];

    /* Initialise cache on first use */
    if (!cache) {
        cache_size = 8;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failure – return 0 */
        /* Copy the first 8 terms */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = INIT[i];
        high_computed = 7;
    }

    /* Make sure the cache can hold the requested index */
    grow_cache((size_t)n + 1);

    /* Compute missing terms only */
    size_t start = high_computed + 1;
    if (start < 8)
        start = 8;                  /* ensure we start from the first unknown term */

    for (size_t idx = start; idx <= (size_t)n; ++idx) {
        /* Recurrence for n ≥ 8 */
        seq_t t1 = cache[idx - 1];
        t1 *= (seq_t)7;

        seq_t t2 = cache[idx - 2];
        t2 *= (seq_t)19;

        seq_t t3 = cache[idx - 3];
        t3 *= (seq_t)26;

        seq_t t4 = cache[idx - 4];
        t4 *= (seq_t)19;

        seq_t t5 = cache[idx - 5];
        t5 *= (seq_t)7;

        seq_t t6 = cache[idx - 6];

        cache[idx] = t1 - t2 + t3 - t4 + t5 - t6;
    }

    high_computed = n;
    return cache[(size_t)n];
}
