/*
 * A165231  a(0)=1, a(1)=8, a(n)=16*a(n-1)-8*a(n-2) for n>1 .
 * https://oeis.org/A165231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, a(n)=16*a(n-1)-8*a(n-2) for n>1 (OEIS A165231)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* memoisation array */
static size_t cap = 0;          /* current allocated capacity */

/* ensure cache has space for indices up to n */
static void grow_cache(idx_t n)
{
    if (n < cap) return;              /* already sufficient */
    size_t new_cap = cap ? cap << 1 : 8;
    if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                 /* allocation failed; unsatisfiable */
    /* initialise new slots to 0 */
    for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise base cases if necessary */
    grow_cache(1);
    if (cache[0] == 0) cache[0] = (seq_t)1;
    if (cache[1] == 0) cache[1] = (seq_t)8;

    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* compute missing values up to n */
    grow_cache(n);
    for (size_t i = 2; i <= (size_t)n; ++i)
    {
        if (cache[i] != 0) continue;  /* already computed */
        seq_t prev1 = cache[i-1];
        seq_t prev2 = cache[i-2];
        cache[i] = (seq_t)16 * prev1 - (seq_t)8 * prev2;
    }
    return cache[n];
}
