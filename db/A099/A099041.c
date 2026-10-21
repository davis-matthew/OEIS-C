/*
 * A099041  Number of 3 X n 0-1 matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (00;1), (10;0) and
 *          (10;1).
 * https://oeis.org/A099041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=8, a(2)=24, a(n)=4*a(n-1)-5*a(n-2)+2*a(n-3) for n>2 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values */
static idx_t  cache_size = 0;        /* number of values stored (cache_size = n+1) */
static size_t cache_capacity = 0;    /* allocated array size */

/* Ensure that a[0..n] are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;  /* already computed */

    /* grow the array to at least n+1 elements */
    if (cache_capacity < (size_t)(n + 1)) {
        size_t new_cap = cache_capacity == 0 ? 4 : cache_capacity * 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;                /* assume realloc succeeds */
        cache_capacity = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = 8;
        else if (i == 2)
            cache[2] = 24;
        else {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            cache[i] = 4 * a1 - 5 * a2 + 2 * a3;
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
