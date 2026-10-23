/*
 * A176023  a(1) = 13, a(n) = square of sum of previous terms.
 * https://oeis.org/A176023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 13; for n >= 2, a(n) = (sum_{k=1}^{n-1} a(k))^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *a_cache = NULL;   /* a_cache[n] = a(n), 1‑based, a_cache[0] -> 0 */
static seq_t *p_cache = NULL;   /* p_cache[n] = sum_{k=1}^n a(k), 1‑based, p_cache[0] -> 0 */
static idx_t cache_cap = 0;     /* allocated capacity (>= cache_max + 1)          */
static idx_t cache_max = 0;     /* largest n that has been computed              */

/* Ensure that the arrays can hold indices up to n (inclusive). */
static void expand_cache_if_needed(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < (size_t)(n + 1))
        new_cap <<= 1;

    if (cache_cap == 0) {
        a_cache = seq_alloc(new_cap);
        p_cache = seq_alloc(new_cap);
    } else {
        a_cache = seq_realloc(a_cache, cache_cap, new_cap);
        p_cache = seq_realloc(p_cache, cache_cap, new_cap);
    }
    cache_cap = new_cap;

    /* initialize sentinel values if this is the first allocation */
    if (cache_max == 0) {
        a_cache[0] = (seq_t)0;
        p_cache[0] = (seq_t)0;
    }
}

/* Compute terms up to n, filling the caches. */
static void ensure_up_to(idx_t n)
{
    if (n <= cache_max)
        return;

    expand_cache_if_needed(n);

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1) {
            a_cache[i] = (seq_t)13;
            p_cache[i] = a_cache[i];
        } else {
            /* a(i) = (sum_{k=1}^{i-1} a(k))^2 = p_cache[i-1]^2 */
            a_cache[i] = p_cache[i - 1] * p_cache[i - 1];
            /* prefix sum update */
            p_cache[i] = p_cache[i - 1] + a_cache[i];
        }
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1, return 0 */
    ensure_up_to(n);
    return a_cache[n];
}
