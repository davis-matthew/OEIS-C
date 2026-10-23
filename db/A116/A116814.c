/*
 * A116814  Number of permutations of length n which avoid the patterns 3124,
 *          3421, 4231.
 * https://oeis.org/A116814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 13 a(n-1) – 72
 *            a(n-2) + 222 a(n-3) – 418 a(n-4) * + 496 a(n-5) – 369 a(n-6) +
 *            165 a(n-7) – 40 a(n-8) + 4 a(n-9) * for n ≥ 10. Initial values
 *            (with a(0)=0) are * a(0)=0, a(1)=1, a(2)=2, a(3)=6, a(4)=21,
 *            a(5)=75, * a(6)=258, a(7)=845, a(8)=2649, a(9)=8019
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t base_values[10] = {0, 1, 2, 6, 21, 75, 258, 845, 2649, 8019};

static const int coeff[9] = {13, -72, 222, -418, 496, -369, 165, -40, 4};

static seq_t *cache        = NULL;   /* a(0) .. a(cache_len-1) */
static size_t cache_len    = 0;       /* number of stored terms */
static size_t cache_cap    = 0;       /* capacity of allocated array */

static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *new_cache = NULL;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache) abort();   /* allocation failed */
    /* copy old data */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    /* free old memory */
#ifdef __cplusplus
    delete [] cache;
#else
    free(cache);
#endif
    cache     = new_cache;
    cache_cap = new_cap;
}

static void ensure_cache(size_t n)
{
    if (n == 0) return;                /* a(0) is already defined as 0 */
    /* extend storage if necessary */
    if (cache_cap < n + 1) grow_cache(n + 1);

    /* fill missing terms */
    size_t i = cache_len;
    while (i <= n) {
        if (i <= 9) {                  /* base values */
            cache[i] = base_values[i];
            ++i;
            continue;
        }
        /* recurrence for i >= 10 */
        seq_t sum = 0;
        for (int k = 0; k < 9; ++k) {
            size_t j = i - 1 - k;      /* a(i-k-1) */
            if (coeff[k] >= 0)
                sum += (seq_t)coeff[k] * cache[j];
            else
                sum -= (seq_t)(-coeff[k]) * cache[j];
        }
        cache[i] = sum;
        ++i;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not part of the sequence but needed for recurrence */
    ensure_cache((size_t)n);
    return cache[(size_t)n];
}
