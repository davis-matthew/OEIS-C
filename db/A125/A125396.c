/*
 * A125396  Number of base 8 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 8*a(n-1) - a(n-2) - 6*a(n-3) (n > 3) Initial values:
 *            a(0)=1, a(1)=8, a(2)=62, a(3)=470
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Internal cache for memoization */
static seq_t *cache = NULL;
static size_t cached = 0;   /* number of stored terms */
static size_t alloc_cap = 0;/* allocated capacity */

/* Ensure the cache can hold n+1 elements (indices 0 .. n). */
static void ensure_capacity(size_t n_idx)
{
    /* n_idx is the maximum index we need (inclusive) */
    size_t needed = n_idx + 1;
    if (needed <= alloc_cap)
        return;
    size_t new_cap = needed > 8 ? needed * 2 : 8;
    seq_t *temp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!temp && new_cap > 0) /* allocation failed */
        exit(1);
    cache = temp;
    alloc_cap = new_cap;
}

/* Main sequence function as required by the build */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;   /* index fits in size_t for our usage */

    /* Load initial terms if cache is empty */
    if (cached == 0) {
        size_t init_cap = 4 > 8 ? 4 : 8;
        if (init_cap < 4) init_cap = 8;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(1);
        alloc_cap = init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)62;
        cache[3] = (seq_t)470;
        cached = 4;
    }

    /* Fast return if we already have the value */
    if (idx < cached)
        return cache[idx];

    /* Expand cache if needed, compute new terms */
    ensure_capacity(idx);
    for (size_t i = cached; i <= idx; ++i) {
        if (i < 4)
            continue;   /* already initialized */
        seq_t val = cache[i - 1] * (seq_t)8;
        val -= cache[i - 2];
        val -= cache[i - 3] * (seq_t)6;
        cache[i] = val;
    }
    cached = idx + 1;
    return cache[idx];
}
