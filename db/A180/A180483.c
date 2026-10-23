/*
 * A180483  Expansion of (3+3*x-25*x^2-3*x^3+2*x^4)/((1-x)*(1-10*x^2+x^4)).
 * https://oeis.org/A180483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-2) - 11*a(n-4) + a(n-6) (a(0)=3, a(1)=6, a(2)=11,
 *            a(3)=38, a(4)=87, a(5)=354)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc, abort */

/* Dynamic array growth helper – works with both C and C++ */
static void grow_cache(idx_t n, seq_t **arr, size_t *cap)
{
    if (n < *cap)
        return;

    size_t new_cap = *cap ? *cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap *= 2;

#ifdef __cplusplus
    seq_t *tmp = reinterpret_cast<seq_t*>(realloc(*arr, new_cap * sizeof(seq_t)));
#else
    seq_t *tmp = realloc(*arr, new_cap * sizeof(seq_t));
#endif
    if (!tmp)
        abort();          /* out‑of‑memory – exit safely */

    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* ---------- static cache ---------- */
    static seq_t *cache       = NULL;    /* dynamic array of values */
    static size_t cache_cap   = 0;       /* allocated size */
    static idx_t  max_store   = (idx_t)-1;/* largest index stored */

    /* ---------- initial known terms ---------- */
    static const seq_t init_vals[6] =
        { (seq_t)3, (seq_t)6, (seq_t)11, (seq_t)38, (seq_t)87, (seq_t)354 };
    const idx_t init_len = 6;

    /* ---------- trivial small indices ---------- */
    if (n < init_len)
        return init_vals[n];

    /* ---------- prepare storage for all indices up to n ---------- */
    grow_cache(n, &cache, &cache_cap);

    /* ---------- initialize cache on first use ---------- */
    if (max_store == (idx_t)-1) {
        for (idx_t i = 0; i < init_len; ++i)
            cache[i] = init_vals[i];
        max_store = init_len - 1;
    }

    /* ---------- already computed? ---------- */
    if (n <= max_store)
        return cache[n];

    /* ---------- extend cache using recurrence ---------- */
    /* Recurrence holds for indices >= 6 */
    for (idx_t i = max_store + 1; i <= n; ++i) {
        cache[i] = cache[i-2] * (seq_t)11
                 - cache[i-4] * (seq_t)11
                 + cache[i-6];
    }

    max_store = n;
    return cache[n];
}
