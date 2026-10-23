/*
 * A169182  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) (1 ≤ n ≤ 26) = 2 + 23 · (a(0)+a(1)+…+a(n−1)) a(27)
 *            = 23 · (a(1)+a(2)+…+a(26)) – 275 a(n) (n ≥ 28) = 23 ·
 *            (a(n−26)+a(n−25)+…+a(n−1)) – 276 · a(n−27) (The latter is the
 *            linear recurrence derived from the rational generating
 *            function.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* The build supplies the types seq_t and idx_t. */
extern seq_t a(idx_t n);

/* Static storage for computed values */
static seq_t *cache      = NULL;   /* array of a values */
static size_t cache_cap  = 0;      /* allocated capacity   */
static size_t cache_len  = 0;      /* number of values computed */

/* Ensure the cache is large enough for indices up to n */
static void
ensure_cache_cap(idx_t n)
{
    size_t needed = (size_t)n + 1;   /* indices 0 … n inclusive */
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap < 16) ? 16 : cache_cap;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Build the first 28 entries using the explicit formulas above. */
static void
init_cache(void)
{
    if (cache_len > 0)
        return;    /* already initialized */

    ensure_cache_cap(27);

    /* a(0) */
    cache[0] = (seq_t)1;
    seq_t sum_prev = cache[0];          /* sum of a(0) … a(k) */

    /* a(1) … a(26) */
    for (idx_t i = 1; i <= 26; ++i) {
        /* a(i) = 2 + 23 * sum_prev */
        cache[i] = (seq_t)2 + (seq_t)23 * sum_prev;
        sum_prev += cache[i];
    }

    /* a(27) = 23 * (sum_{k=1}^{26} a(k)) – 275
              sum_{k=1}^{26} a(k) = sum_prev – a(0)  */
    cache[27] = (seq_t)23 * (sum_prev - cache[0]) - (seq_t)275;

    cache_len = 28;  /* we have computed indices 0 … 27 */
}

/* Compute a(k) for all k up to n if necessary */
static void
compute_to(idx_t n)
{
    init_cache();                 /* ensure base values exist */

    if ((size_t)n < cache_len)
        return;                   /* already computed */

    for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
        /* a(k) for k ≥ 28 */
        seq_t sum_26 = (seq_t)0;
        for (idx_t j = k - 26; j < k; ++j)
            sum_26 += cache[j];

        cache[k] = (seq_t)23 * sum_26 - (seq_t)276 * cache[k - 27];
    }
    cache_len = (size_t)n + 1;
}

seq_t
a(idx_t n)
{
    ensure_cache_cap(n);
    compute_to(n);
    return cache[(size_t)n];
}
