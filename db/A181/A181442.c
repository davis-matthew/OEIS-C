/*
 * A181442  Expansion of (1 + x - 8*x^2 + x^3 + x^4) / ( (1 - x)*(1 - 10*x^2 +
 *          x^4) ).
 * https://oeis.org/A181442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A181442: Expansion of (1 + x - 8*x^2 + x^3 + x^4) /
 *                ((1 - x)*(1 - 10*x^2 + x^4))
 *
 * Recurrence used:
 *   a(n) = a(n-1) + 10*a(n-2) - 10*a(n-3) - a(n-4) + a(n-5)
 *   with initial values
 *   a(0)=1, a(1)=2, a(2)=4, a(3)=15, a(4)=35
 *
 * The implementation memoises computed values in a dynamically sized
 * static array so repeated calls are efficient and works for any n ≥ 0.
 */
#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static memoisation storage */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len  = 0;

    /* Initialisation: base values up to a(4) */
    if (cache == NULL) {
        /* Allocate some room, grow as needed */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        /* Base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)35;
        cache_len = 5;   /* next index to compute */
    }

    if (n < cache_len)
        return cache[n];

    /* Grow cache capacity if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap << 1;
        if (new_cap <= n)
            new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; abort to avoid undefined behavior */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Compute a(i) = a(i-1) + 10*a(i-2) - 10*a(i-3) - a(i-4) + a(i-5) */
        seq_t term1 = cache[i-1];
        term1 += ((seq_t)10) * cache[i-2];
        term1 += cache[i-5];

        seq_t term2 = ((seq_t)10) * cache[i-3];
        term2 += cache[i-4];

        cache[i] = term1 - term2;
    }
    cache_len = n + 1;
    return cache[n];
}
