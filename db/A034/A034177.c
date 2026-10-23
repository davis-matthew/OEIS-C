/*
 * A034177  a(n) is the n-th quartic factorial number divided by 4.
 * https://oeis.org/A034177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^(n-1) * n! (or 4*a(n) = 4^n * n!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1 ; a(n)=4*n*a(n-1) for n>1 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of precomputed values */
static size_t cache_size = 0;    /* number of elements stored */

/* Ensure that cache contains values up to a(n) */
static void _ensure_cache(idx_t n)
{
    if (cache_size >= n) return;

    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return;           /* out of memory → keep old cache */

    cache = tmp;

    if (cache_size == 0) {
        cache[0] = (seq_t)1;    /* a(1) = 1 */
        cache_size = 1;
    }

    seq_t val = cache[cache_size - 1];
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t factor = (seq_t)i * (seq_t)4;
        val = val * factor;
        cache[i - 1] = val;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for 0, return 0 as safety */
    _ensure_cache(n);
    return cache[n - 1];
}
