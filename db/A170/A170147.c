/*
 * A170147  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170147: Number of reduced words of length n in a Coxeter
 * group on 42 generators with relations (S_i)^2 = (S_i S_j)^38 = I.
 *
 * Generating function:
 *   G(t) = (t^38 + 2 t^37 + 2 t^36 + … + 2 t + 1) /
 *          (820 t^38 – 40 t^37 – 40 t^36 – … – 40 t + 1).
 *
 * This leads to the linear recurrence
 *
 *   a(0) = 1,
 *   a(n) = 2 + 40 * a(n-1) + 40 * a(n-2) + … + 40 * a(n-37)
 *          – 820 * a(n-38)            for 1 ≤ n ≤ 37,
 *
 *   a(38) = 1 + 40 * a(37) + … + 40 * a(1) – 820 * a(0),
 *
 *   a(n) = 0 + 40 * a(n-1) + … + 40 * a(n-37) – 820 * a(n-38)
 *          for n ≥ 39,
 *
 * where missing indices are treated as 0.
 * This implementation memoises computed terms for speed.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;        /* Array holding computed terms   */
static size_t a_cache_used = 0;      /* Number of terms already stored */
static size_t a_cache_capacity = 0;  /* Allocated capacity */

static void *ensure_cache(size_t needed)
{
    if (needed <= a_cache_capacity)
        return a_cache;

    size_t new_cap = a_cache_capacity ? a_cache_capacity : 1;
    while (new_cap < needed)
        new_cap *= 2;

    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    /* initialise new elements to 0 */
    memset((char *)a_cache + a_cache_capacity * sizeof(seq_t),
           0, (new_cap - a_cache_capacity) * sizeof(seq_t));
    a_cache_capacity = new_cap;
    return a_cache;
}

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough */
    ensure_cache((size_t)n + 1);

    /* Compute missing terms up to n */
    for (size_t i = a_cache_used; i <= (size_t)n; ++i)
    {
        seq_t val = 0;

        /* Constant term from the numerator */
        if (i == 0)
            val = (seq_t)1;
        else if (i <= 37)
            val = (seq_t)2;
        else if (i == 38)
            val = (seq_t)1;

        /* Add 40 * a[i-j] for j = 1..37 */
        for (int j = 1; j <= 37; ++j)
        {
            if (i >= (size_t)j)
                val += (seq_t)40 * a_cache[i - j];
        }

        /* Subtract 820 * a[i-38] when i ≥ 38 */
        if (i >= 38)
            val -= (seq_t)820 * a_cache[i - 38];

        a_cache[i] = val;
    }

    a_cache_used = (size_t)n + 1;
    return a_cache[(size_t)n];
}
