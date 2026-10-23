/*
 * A158532  a(n) = Hermite(n,9).
 * https://oeis.org/A158532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Hermite(n,9) Recurrence: a(0)=1, a(1)=18, a(n) =
 *            18*a(n-1) - 2*(n-1)*a(n-2) for n>=2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 15, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* static cache for already computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity in elements   */
static size_t cache_size = 0;  /* number of valid elements filled */

/* enlarge cache to hold at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failure */
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* compute the n-th term of OEIS sequence A158532 (Hermite(n,9)) */
seq_t a(idx_t n)
{
    /* index must be non‑negative; cast to size_t (value fits for n<=10000) */
    size_t idx = (size_t)n;

    /* initialise cache with base values if it hasn't been done yet */
    if (cache_size == 0) {
        ensure_cache_cap(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)18;
        cache_size = 2;
    }

    /* simple result retrieval if already computed */
    if (idx < cache_size)
        return cache[idx];

    /* make sure we have space for required elements */
    ensure_cache_cap(idx + 1);

    /* compute missing terms iteratively using the recurrence */
    for (size_t i = cache_size; i <= idx; ++i) {
        /* H_i = 18*H_{i-1} - 2*(i-1)*H_{i-2} */
        seq_t val = (seq_t)18 * cache[i - 1]
                    - (seq_t)(2 * (i - 1)) * cache[i - 2];
        cache[i] = val;
    }
    cache_size = idx + 1;
    return cache[idx];
}
