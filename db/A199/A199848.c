/*
 * A199848  Number of -n..n arrays x(0..3) of 4 elements with zero sum and no
 *          element more than one greater than the previous.
 * https://oeis.org/A199848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6) for n ≥ 7 Initial values (offset 1): a(1)=11, a(2)=23,
 *            a(3)=42, a(4)=68, a(5)=103, a(6)=149
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache of computed terms        */
static size_t cache_cap = 0;  /* current allocated size in terms (index n+1) */

/* Ensure the cache can hold terms up to n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;          /* already big enough */

    size_t new_cap = (size_t)n + 1;             /* include index 0 */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);              /* allocation failed */

    /* Initialize newly allocated entries to 0.            */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;

    cache     = tmp;
    cache_cap = new_cap;

    /* Set base terms (they are the same for all sizes).  */
    for (idx_t i = 1; (idx_t)i < (idx_t)new_cap; ++i) {
        switch (i) {
            case 1: cache[i] = (seq_t)11;  break;
            case 2: cache[i] = (seq_t)23;  break;
            case 3: cache[i] = (seq_t)42;  break;
            case 4: cache[i] = (seq_t)68;  break;
            case 5: cache[i] = (seq_t)103; break;
            case 6: cache[i] = (seq_t)149; break;
            default: break;              /* leave zero */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence defined for n ≥ 1 */

    ensure_cache(n);

    if (n <= 6) return cache[(size_t)n];

    /* Compute missing terms using the recurrence.  */
    for (idx_t i = 7; i <= n; ++i) {
        if (cache[(size_t)i] != 0) continue;  /* already computed */

        seq_t t1 = 3 * cache[(size_t)(i - 1)];
        seq_t t2 = 3 * cache[(size_t)(i - 2)];
        seq_t t3 = 2 * cache[(size_t)(i - 3)];
        seq_t t4 = 3 * cache[(size_t)(i - 4)];
        seq_t t5 = 3 * cache[(size_t)(i - 5)];
        seq_t t6 =     cache[(size_t)(i - 6)];

        cache[(size_t)i] = (t1 + t3 + t5) - (t2 + t4 + t6);
    }

    return cache[(size_t)n];
}
