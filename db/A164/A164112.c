/*
 * A164112  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = -820*a(n-6) + 40*Sum_{k=1..5} a(n-k) * with initial
 *            values a(0)=1, a(1)=42, a(2)=1722, a(3)=70602, * a(4)=2894682,
 *            a(5)=118681962, a(6)=4865959581
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   seq_alloc, seq_from_str, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_from_str.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap = 0;

/* Ensure that cache has at least 'need' elements.  */
static void ensure_capacity(idx_t need)
{
    if (need < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= need)
        new_cap <<= 1;               /* double until sufficient */
    seq_t *new_ptr = seq_realloc(cache,
                                 cache_cap ? cache_cap : 0,
                                 new_cap);
    if (!new_ptr)
        abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Initialise cache with the first seven terms. */
static void init_cache(void)
{
    if (cache)
        return;
    cache = seq_alloc(16);      /* allocate space */
    if (!cache)
        abort();
    cache_cap = 16;

    /* The first seven terms as decimal strings. */
    static const char *base_str[7] = {
        "1",
        "42",
        "1722",
        "70602",
        "2894682",
        "118681962",
        "4865959581"
    };
    for (idx_t i = 0; i < 7; ++i)
        cache[i] = seq_from_str(base_str[i]);
    cache_size = 7;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_size)
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = cache_size; i <= n; ++i) {
        /* sum of the previous five terms */
        seq_t sum = cache[i-1] + cache[i-2];
        sum += cache[i-3];
        sum += cache[i-4];
        sum += cache[i-5];
        /* compute the recurrence: 40*sum - 820*a(i-6) */
        seq_t term = 40 * sum;
        term -= 820 * cache[i-6];
        cache[i] = term;
    }

    cache_size = n + 1;
    return cache[n];
}
