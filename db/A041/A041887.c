/*
 * A041887  Denominators of continued fraction convergents to sqrt(465).
 * https://oeis.org/A041887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 31742*a(n-10) - a(n-20) for n>19 Initial terms
 *            a(0)...a(19) are given below
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init_vals[20] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)7, (seq_t)16, (seq_t)39,
    (seq_t)94, (seq_t)321, (seq_t)415, (seq_t)736,
    (seq_t)31327, (seq_t)32063, (seq_t)63390, (seq_t)222233, (seq_t)507856,
    (seq_t)1237945, (seq_t)2983746, (seq_t)10189183, (seq_t)13172929, (seq_t)23362112
};

static const seq_t p31742 = (seq_t)31742;

/* Cache for computed terms. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_size = 0;   /* number of values stored (highest index+1) */

/* Ensure the cache can hold terms up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : required;
    while (new_cap < required) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute all terms up to and including n. */
static void compute_up_to(idx_t n)
{
    size_t target = (size_t)n;
    if (cache_size > target) return;   /* already done */
    for (size_t i = cache_size; i <= target; ++i)
    {
        if (i < 20)
            cache[i] = init_vals[i];
        else
            cache[i] = p31742 * cache[i - 10] - cache[i - 20];
    }
    cache_size = target + 1;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    compute_up_to(n);
    return cache[(size_t)n];
}
