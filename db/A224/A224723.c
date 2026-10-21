/*
 * A224723  Number of (n+3) X 6 0..2 matrices with each 4 X 4 subblock
 *          idempotent.
 * https://oeis.org/A224723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 4*a(n-4) * - 3*a(n-5) +
 *            2*a(n-7) - a(n-8), for n > 10 * Initial values: a(1)=2471,
 *            a(2)=2493, a(3)=3092, a(4)=3502, * a(5)=3775, a(6)=4868,
 *            a(7)=6540, a(8)=8551, * a(9)=10761, a(10)=13991
 * Range:     u8 none, u16 n <= 15, u32 n <= 49, u64 n <= 118, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ---------- static data ---------- */
static const seq_t init_values[10] = {
    (seq_t)2471, (seq_t)2493, (seq_t)3092, (seq_t)3502, (seq_t)3775,
    (seq_t)4868, (seq_t)6540, (seq_t)8551, (seq_t)10761, (seq_t)13991
};

static seq_t *cache = NULL;      /* pointer to cached terms */
static size_t cache_cap = 0;     /* allocated capacity (number of terms) */
static size_t cache_len = 0;     /* number of terms currently stored */

/* ---------- helper: grow cache capacity ---------- */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);  /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* inteded for safety, not part of sequence */

    /* Initialise cache with known first ten terms on first use. */
    if (cache_len == 0) {
        grow_cache(10);
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init_values[i];
        cache_len = 10;
    }

    size_t sn = (size_t)n;          /* n fits into size_t for our limits */

    /* If already computed, return immediately. */
    if (sn <= cache_len)
        return cache[sn - 1];

    /* Compute missing terms up to n. */
    for (size_t i = cache_len + 1; i <= sn; ++i) {
        /* recurrence indices are 1-based. */
        seq_t val =   (seq_t)3 * cache[i - 2]          /* a(i-1) */
                   - (seq_t)2 * cache[i - 3]          /* a(i-2) */
                   - (seq_t)2 * cache[i - 4]          /* a(i-3) */
                   + (seq_t)4 * cache[i - 5]          /* a(i-4) */
                   - (seq_t)3 * cache[i - 6]          /* a(i-5) */
                   + (seq_t)2 * cache[i - 8]          /* a(i-7) */
                   -      cache[i - 9];               /* a(i-8) */
        grow_cache(i);
        cache[i - 1] = val;
    }
    cache_len = sn;
    return cache[sn - 1];
}
