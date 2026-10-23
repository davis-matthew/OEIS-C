/*
 * A170190  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = p_n + 35 *
 *            (a(n-1)+a(n-2)+…+a(n-38)) – 630 * a(n-39) * where p_0 = p_39 =
 *            1, p_1 … p_38 = 2, and p_n = 0 for n>39. * This follows from the
 *            generating function * (t^39 + 2t^38 + … + 2t + 1) / (630t^39 –
 *            35t^38 – … – 35t + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* memoised sequence values */
static size_t cache_cap = 0;         /* current allocated capacity */
static idx_t  computed_to = (idx_t)-1;  /* highest index computed so far */

/* Grow the cache to be able to store 'need' (0‑based) entries. */
static void
ensure_capacity(idx_t need)
{
    size_t required = (size_t)(need + 1);
    if (required <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < required) new_cap = required;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* ensure we have room for the requested index */
    ensure_capacity(n);

    /* compute terms up to 'n' that haven't been computed yet */
    idx_t start = computed_to + 1;   /* unsigned wrap‑around gives 0 if computed_to==UINT64_MAX */
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t sum = 0;
        /* add 35 * a(i-k) for k = 1 .. min(38,i) */
        for (idx_t k = 1; k <= 38 && k <= i; ++k)
            sum += (seq_t)35 * cache[i - k];

        if (i >= 39)
            sum -= (seq_t)630 * cache[i - 39];

        seq_t p = 0;
        if (i == 0 || i == 39)        /* numerator coefficient */
            p = 1;
        else if (i <= 38 && i >= 1)
            p = 2;

        cache[i] = p + sum;
    }

    computed_to = n;   /* all indices up to n are now available */
    return cache[n];
}
