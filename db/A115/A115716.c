/*
 * A115716  A divide-and-conquer sequence.
 * https://oeis.org/A115716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1021, u32 n <= 7632, u64 n <= 7632,
 *            u128 n <= 7632, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A115716
   Recurrence:
      a(0) = 1;
      a(2*n + 1) = 1  (n ≥ 0);
      a(2*n + 2) = 4*a(n) – 1  (n ≥ 0) */

/* Dependencies: seq_t and idx_t types are provided by the build system.
   The function uses only integer arithmetic and standard library
   facilities.  It maintains a static cache so that successive calls
   (in any order) are fast. */

#include <stdlib.h>   /* malloc, realloc, abort */

/* Ensure the cache array has at least 'need' elements.
   Works for both C and C++ builds. */
static void ensure_cache(idx_t need, seq_t **arr, idx_t *cap)
{
    if (need <= *cap)                     /* enough space already   */
        return;
    idx_t new_cap = (*cap == 0 ? 1 : *cap);
    while (new_cap < need)
        new_cap <<= 1;                    /* grow by factors of 2   */
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (*arr == NULL)
        abort();                          /* terminate on failure   */
    *cap = new_cap;
}

/* Compute a(n) for any non‑negative n. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values. */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed_lim = 0;   /* highest index computed so far */

    /* First call – initialise base cases a(0)=1, a(1)=1. */
    if (cache == NULL) {
        ensure_cache(2, &cache, &cache_cap);   /* space for 0 and 1 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        computed_lim = 1;
    }

    /* Grow the cache if needed for the current n. */
    if (n + 1 > cache_cap)
        ensure_cache(n + 1, &cache, &cache_cap);

    /* Compute any missing values up to n. */
    for (idx_t i = computed_lim + 1; i <= n; ++i) {
        if ((i & 1) == 1)                     /* odd index   */
            cache[i] = (seq_t)1;
        else {                                /* even index  */
            idx_t k = (i - 2) >> 1;            /* i = 2*k+2   */
            cache[i] = (seq_t)4 * cache[k] - (seq_t)1;
        }
    }
    if (n > computed_lim)
        computed_lim = n;

    return cache[n];
}
