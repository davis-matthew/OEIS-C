/*
 * A127220  a(n) = 3^n*tetranacci(n) or (2^n)*A001648(n).
 * https://oeis.org/A127220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 9*a(n-2) + 27*a(n-3) + 81*a(n-4) with a(1) =
 *            3, a(2) = 27, a(3) = 189, a(4) = 1215 (Offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed terms. */
static seq_t *cache = NULL;   /* 1‑based indexing; cache[0] unused */
static size_t cache_size = 0; /* allocated entries count */
static size_t last_computed = 0; /* largest n for which cache[n] is valid */

/* Resize cache to hold at least n+1 entries. */
static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n)
        return;
    size_t new_size = cache_size == 0 ? 5 : cache_size;
    while (new_size <= (size_t)n)
        new_size *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Omitted: real code would handle out‑of‑memory appropriately. */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_size = new_size;
}

/* Initialize the first four terms if not already done. */
static void init_terms(void)
{
    if (last_computed >= 4)
        return;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)27;
    cache[3] = (seq_t)189;
    cache[4] = (seq_t)1215;
    last_computed = 4;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* n >= 1 by problem statement; behaviour for n < 1 is undefined */
    ensure_cache(n);
    init_terms();
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* a(i) = 3*a(i-1) + 9*a(i-2) + 27*a(i-3) + 81*a(i-4) */
            seq_t v1 = (seq_t)3  * cache[i - 1];
            seq_t v2 = (seq_t)9  * cache[i - 2];
            seq_t v3 = (seq_t)27 * cache[i - 3];
            seq_t v4 = (seq_t)81 * cache[i - 4];
            cache[i] = v1 + v2 + v3 + v4;
        }
        last_computed = n;
    }
    return cache[n];
}
