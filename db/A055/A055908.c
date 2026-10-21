/*
 * A055908  Column 2 of triangle A055907.
 * https://oeis.org/A055908
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 4*a(n-4) - 4*a(n-5) +
 *            2*a(n-6) + 2*a(n-7) - 3*a(n-8) + a(n-9) + (n==2||n==3||n==5 ? 1
 *            : 0) for n >= 2 (a(0)=a(1)=0)
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* length of the array */
static size_t computed = 1;          /* largest index that has been computed */

/* Ensure the cache can hold values up to 'n' */
static void ensure_cache(size_t n)
{
    if (n < cache_len)
        return;

    size_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failure: abort the program (no I/O allowed) */
        exit(EXIT_FAILURE);
    }
    /* Initialize new elements to zero */
    for (size_t i = cache_len; i < new_len; ++i)
        new_cache[i] = 0;

    cache     = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                    /* indices below offset return 0 */

    ensure_cache(n);

    /* Compute any missing terms up to n */
    for (size_t k = computed + 1; k <= n; ++k) {
        seq_t val = 3 * cache[k - 1];

        if (k >= 2) val -= 2 * cache[k - 2];
        if (k >= 3) val -= 2 * cache[k - 3];
        if (k >= 4) val += 4 * cache[k - 4];
        if (k >= 5) val -= 4 * cache[k - 5];
        if (k >= 6) val += 2 * cache[k - 6];
        if (k >= 7) val += 2 * cache[k - 7];
        if (k >= 8) val -= 3 * cache[k - 8];
        if (k >= 9) val +=      cache[k - 9];

        if (k == 2 || k == 3 || k == 5)
            val += (seq_t)1;

        cache[k] = val;
    }
    computed = (size_t)n;
    return cache[n];
}
