/*
 * A041127  Denominators of continued fraction convergents to sqrt(72).
 * https://oeis.org/A041127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 34*a(n-2) - a(n-4) for n≥4; a(0)=1, a(1)=2, a(2)=33,
 *            a(3)=68
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* realloc, exit */

static seq_t *cache = NULL;        /* array holding computed terms   */
static idx_t cache_len = 0;        /* number of terms stored         */
static idx_t cache_cap = 0;        /* current allocated capacity     */

/* Ensure that cache has entries 0 … n available.  Compute any that
   are missing using the recurrence.  */
static void ensure_cache(idx_t n)
{
    idx_t need = n + 1;                 /* number of terms required     */
    if (cache_len >= need) return;      /* already have them             */

    /* Grow the buffer; start with 4 and double until big enough.  */
    size_t new_cap = cache_cap > 0 ? (size_t)cache_cap : 4;
    while ((size_t)new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { cache = NULL; cache_cap = 0; cache_len = 0; return; }
    cache = tmp;
    cache_cap = (idx_t)new_cap;

    /* Compute any new entries.  */
    for (idx_t i = cache_len; i < need; ++i) {
        if (i == 0)          cache[0] = 1;
        else if (i == 1)     cache[1] = 2;
        else if (i == 2)     cache[2] = 33;
        else if (i == 3)     cache[3] = 68;
        else
            cache[i] = 34 * cache[i-2] - cache[i-4];
    }
    cache_len = need;
}

/* Public function required by the test harness.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
