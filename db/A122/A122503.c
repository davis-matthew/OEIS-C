/*
 * A122503  a(n) = 22*a(n - 2) + 54*a(n - 3) + 38*a(n - 4).
 * https://oeis.org/A122503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A122503
 * Recurrence:  a(n) = 22*a(n-2) + 54*a(n-3) + 38*a(n-4)
 * Offset: 1   (a(1) is the first term)
 */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache   = NULL;   /* dynamic cache of computed terms */
static size_t cap = 0;          /* current capacity of `cache` */
static idx_t  last_computed = 0;/* highest index that has been computed */

seq_t a(idx_t n)
{
    if (n < 1)            /* behaviour below the offset is irrelevant */
        return 0;

    /* Ensure the cache can hold indices up to n+4 (guarantees space for i-4). */
    size_t needed = (size_t)n + 4;
    if (needed > cap) {
        size_t newcap = cap ? cap << 1 : 8;   /* double the size or start at 8 */
        if (newcap < needed)
            newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {                  /* allocation failure – abort */
            return 0;
        }
        cache = tmp;
        cap = newcap;
    }

    /* Initialise the base case terms only once. */
    if (last_computed == 0) {
        cache[1] = cache[2] = cache[3] = cache[4] = (seq_t)1;
        last_computed = 4;
    }

    /* If the requested term is already known, return it. */
    if (n <= last_computed)
        return cache[n];

    /* Compute all terms up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 4) {
            cache[i] = (seq_t)1;
        } else {
            cache[i] =
                (seq_t)22 * cache[i-2] +
                (seq_t)54 * cache[i-3] +
                (seq_t)38 * cache[i-4];
        }
    }
    last_computed = n;
    return cache[n];
}
