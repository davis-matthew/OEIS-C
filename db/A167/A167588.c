/*
 * A167588  The second column of the ED4 array A167584.
 * https://oeis.org/A167588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) + (2*n-5)*(2*n-1)*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) is the first term of the OEIS sequence. For n < 1 the
   function returns 0; such calls are harmless. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache to support arbitrary order of calls */
static seq_t *cache = NULL;      /* array holding computed values */
static size_t cache_cap = 0;     /* allocated elements */
static size_t cache_len = 0;     /* maximal computed index + 1 */

/* Ensure that the cache is large enough for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;           /* already sufficient */

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */

    /* realloc may return NULL on allocation failure */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                            /* terminate on OOM */

    /* Zero‑initialize newly allocated slots (works for both arithmetic
       types and class types with an assignment operator). */
    if (new_cap > cache_cap) {
        for (size_t i = cache_cap; i < new_cap; ++i) {
            tmp[i] = (seq_t)0;
        }
    }

    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize the first two terms of the sequence. */
static void init_base(void)
{
    if (cache_len >= 2) return;   /* already initialized */

    ensure_cache(1);              /* guarantee slots 0 and 1 exist */

    cache[0] = (seq_t)0;          /* a(0) – not part of OEIS offset */
    cache[1] = (seq_t)1;          /* a(1) */
    cache_len = 2;                /* next free index is 2 */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* n=0 is not part of the OEIS offset */

    init_base();                  /* ensure a(0) and a(1) are set */

    /* Compute missing terms up to n if necessary. */
    if ((size_t)n >= cache_len) {
        ensure_cache(n);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* Recurrence: a(i) = 6*a(i-1) + (2*i-5)*(2*i-1)*a(i-2)   */
            seq_t term1 = (seq_t)6 * cache[i - 1];
            seq_t term2 = cache[i - 2] *
                           (seq_t)(2 * (idx_t)i - 5) *
                           (seq_t)(2 * (idx_t)i - 1);
            cache[i] = term1 + term2;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
