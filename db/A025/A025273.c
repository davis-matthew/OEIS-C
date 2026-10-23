/*
 * A025273  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 5,
 *          starting 1,0,1,1.
 * https://oeis.org/A025273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 27, u64 n <= 48, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025273 */
/*     a(1) = 1, a(2) = 0, a(3) = 1, a(4) = 1;     */
/*     a(n) = Σ_{i=1}^{n-1} a(i) * a(n-i)   for n ≥ 5   */

#include <stdlib.h>   /* malloc / realloc / exit */

static seq_t *cache = NULL;   /* cache[0] is unused, cache[1].. are valid values */
static idx_t  cache_len = 0;   /* largest index currently stored */

/* Ensure the cache can hold indices 1 … n (n ≥ 1).  Any new entries
 * are initialised to zero.  This routine also compacts allocation to
 * the requested size. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);           /* out‑of‑memory – terminate */

    cache = tmp;
    for (idx_t i = cache_len; i <= n; ++i)
        cache[i] = 0;

    cache_len = n;
}

/* Populate the first four terms (required only when n ≥ 5). */
static void init_first_four(void)
{
    if (cache[1] != 0)   /* already initialised once */
        return;

    cache[1] = (seq_t)1;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)1;
    cache[4] = (seq_t)1;
}

/* --------------------------------------------------------------- */
/*   Main public routine                                               */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* the sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* Provide the fixed first four values directly. */
    if (n <= 4) {
        static const seq_t first[5] = {0,
                                      (seq_t)1,  /* a(1) */
                                      (seq_t)0,  /* a(2) */
                                      (seq_t)1,  /* a(3) */
                                      (seq_t)1   /* a(4) */
                                     };
        return first[n];
    }

    /* Ensure the cache holds at least up to n and initialise first
     * four terms if not done already. */
    ensure_cache(n);
    init_first_four();

    /* Compute all terms up to n that have not been computed yet. */
    for (idx_t k = 5; k <= n; ++k) {
        if (cache[k] != 0)
            continue;               /* already computed */

        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i)
            sum += cache[i] * cache[k - i];

        cache[k] = sum;
    }

    return cache[n];
}
