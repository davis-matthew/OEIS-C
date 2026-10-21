/*
 * A228535  x-values in the solution to the Pell equation x^2 - 53*y^2 = -1.
 * https://oeis.org/A228535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 132498*a(n-1) - a(n-2) with a(1) = 182, a(2) = 24114818
 *            (offset 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Expand cache size to cover at least n+1 entries (index 0 unused) */
static void ensure_cache(size_t n)
{
    if (cache_cap < n + 1) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_buf) exit(1);
        for (size_t i = 0; i < cache_cap; i++) new_buf[i] = cache[i];
        free(cache);
        cache = new_buf;
        cache_cap = new_cap;
    }
    /* Initialize known base terms if not already set */
    if (n >= 1 && cache[1] == 0) cache[1] = (seq_t)182;
    if (n >= 2 && cache[2] == 0) cache[2] = (seq_t)24114818;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if ((idx_t)0 >= n) return (seq_t)0;  /* undefined for n < 1, return 0 */
    size_t idx = (size_t)n;
    ensure_cache(idx);
    if (idx <= 2) return cache[idx];

    for (size_t i = 3; i <= idx; i++) {
        if (cache[i] != 0) continue; /* already computed */
        cache[i] = (cache[i - 1] * (seq_t)132498) - cache[i - 2];
    }
    return cache[idx];
}
