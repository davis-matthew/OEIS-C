/*
 * A050650  a(n+1) is next smallest cube ending with a(n), initial term is 27.
 * https://oeis.org/A050650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (3 + Σ_{k=1}^{n-1} 10^{len(a(k))})^3 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

static const seq_t get_pow10(unsigned int e) {
    static seq_t power10[32];
    static bool init = false;
    if (!init) {
        power10[0] = (seq_t)1;
        for (unsigned int i = 1; i < 32; ++i)
            power10[i] = power10[i-1] * (seq_t)10;
        init = true;
    }
    return power10[e];
}

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

static seq_t *cache = NULL;       /* cached terms a(1)...a(max_cached)      */
static size_t cache_cap = 0;      /* capacity of the cache                 */
static idx_t  max_cached = 0;     /* number of terms currently cached       */
static seq_t  next_root  = 3;     /* cube root of the next term (a_{max+1}) */

/* Ensure the cache can hold at least `need` elements.  Grow if necessary. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    seq_t *new_arr = alloc_seq_array(need);
    if (!new_arr)
        return;                       /* allocation failure – abort silently */

    for (size_t i = 0; i < max_cached; ++i)
        new_arr[i] = cache[i];

    if (cache)
        free_seq_array(cache);

    cache      = new_arr;
    cache_cap  = need;
}

/* Return the nth term of A050650.  The sequence is 1‑indexed. */
seq_t a(idx_t n)
{
    if (n == 0)            /* sequence is defined for n ≥ 1; return 0 for safety */
        return 0;

    if (n <= max_cached)   /* already have the result */
        return cache[n - 1];

    /* Grow cache to the required size */
    ensure_cache((size_t)n);

    /* Compute missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        /* Current cube root */
        seq_t root  = next_root;
        /* Term = root^3 */
        seq_t term  = root * root * root;

        /* Store the term */
        cache[i - 1] = term;

        /* Prepare root for the next term:
           next_root = root + 10^{len(term)}      */
        int len   = num_digits(term);                    /* number of decimal digits */
        seq_t pow10 = get_pow10((unsigned)len);
        next_root = root + pow10;
    }

    max_cached = n;
    return cache[n - 1];
}
