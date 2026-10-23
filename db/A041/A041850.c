/*
 * A041850  Numerators of continued fraction convergents to sqrt(447).
 * https://oeis.org/A041850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 296*a(n-2) - a(n-4) Initial values: a(0)=21, a(1)=148,
 *            a(2)=6237, a(3)=43807
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute the nth term of OEIS sequence A041850 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)21;
    if (n == 1) return (seq_t)148;
    if (n == 2) return (seq_t)6237;
    if (n == 3) return (seq_t)43807;

    /* Static cache for computed values */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cache_cap  = 0;     /* allocated capacity */
    static size_t computed_len = 0;   /* number of terms already computed */

    /* Helper to grow the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise cache with first four values if not yet done */
    if (computed_len == 0) {
        cache[0] = (seq_t)21;
        cache[1] = (seq_t)148;
        cache[2] = (seq_t)6237;
        cache[3] = (seq_t)43807;
        computed_len = 4;
    }

    /* Compute terms up to requested index */
    for (size_t i = computed_len; i <= (size_t)n; ++i) {
        /* Recurrence: a(i) = 296*a(i-2) - a(i-4) */
        seq_t prod = (seq_t)296 * cache[i - 2];
        cache[i] = prod - cache[i - 4];
    }
    computed_len = (size_t)n + 1; /* all terms up to n are now stored */

    return cache[(size_t)n];
}
