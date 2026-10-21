/*
 * A216034  a(n) = 3^(-1+floor((n+1)/3))*A(n), where A(n) = 3*A(n-1) + A(n-2)
 *          - A(n-3)/3 with A(0)=A(1)=3, A(2)=11.
 * https://oeis.org/A216034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 105*a(n-3) + 33*a(n-6) - a(n-9) for n ≥ 9 Initial values:
 *            a(0)=1, a(1)=1, a(2)=11, a(3)=35, a(4)=115, a(5)=1129,
 *            a(6)=3697, a(7)=12105, a(8)=118907
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* computed terms                */
static idx_t  cache_size = 0;       /* number of elements stored     */

static const seq_t init_terms[9] = {
    1, 1, 11, 35, 115, 1129, 3697, 12105, 118907
};

/* Ensure that cache contains elements [0..n]; extend if necessary.  */
static void expand_cache(idx_t n)
{
    if (cache && n < cache_size) return;          /* already present */

    /* allocate (or enlarge) the cache */
    idx_t new_size = n + 1;
    if (!cache) {
        cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    }
    /* set initial terms if they are within the new range */
    for (idx_t i = 0; i < 9 && i < new_size; ++i) {
        cache[i] = init_terms[i];
    }

    /* compute missing terms up to n */
    idx_t start = cache_size;
    if (start < 9) start = 9;        /* terms <9 are already known */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = 105 * cache[i - 3] + 33 * cache[i - 6] - cache[i - 9];
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    expand_cache(n);
    return cache[n];
}
