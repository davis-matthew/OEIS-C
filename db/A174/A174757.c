/*
 * A174757  x-values in the solution to x^2-53*y^2=1.
 * https://oeis.org/A174757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 132498*a(n-1) - a(n-2),
   with a(1) = 1, a(2) = 66249. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* array holding computed terms */
static size_t cache_len = 0;         /* number of terms currently cached */
static size_t cache_cap = 0;         /* current allocated capacity */

/* Ensure the cache can hold at least `need` terms.  */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();           /* out‑of‑memory */
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* undefined, but safe */
    size_t idx = (size_t)n;                /* n fits in a size_t for our bounds */

    /* Grow cache up to n terms */
    ensure_capacity(idx);

    /* Compute missing terms */
    while (cache_len < idx) {
        if (cache_len == 0) {
            cache[0] = (seq_t)1;           /* a(1) */
        } else if (cache_len == 1) {
            cache[1] = (seq_t)66249;       /* a(2) */
        } else {
            /* a(n) = 132498*a(n-1) - a(n-2) */
            seq_t prev  = cache[cache_len - 1];
            seq_t prev2 = cache[cache_len - 2];
            cache[cache_len] = (seq_t)132498 * prev - prev2;
        }
        ++cache_len;
    }

    return cache[n-1];
}
