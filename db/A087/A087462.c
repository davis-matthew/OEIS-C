/*
 * A087462  Generalized mod 3 multiplicative Jacobsthal sequence.
 * https://oeis.org/A087462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*a(n-3) + 8*a(n-6) for n > 5 a(0) = 1, a(1) = 1, a(2) =
 *            1, a(3) = 8, a(4) = 5, a(5) = 11 (This uses the recurrence given
 *            in the OEIS entry.)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 32, u64 n <= 65, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, exit */

static seq_t *a_cache = NULL;    /* cached values of the sequence   */
static idx_t cache_len = 0;      /* number of valid entries          */
static idx_t cache_cap = 0;      /* allocated capacity (in elements) */

/* Ensure that a_cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* First allocation or resize if necessary. */
    if (a_cache == NULL) {
        /* Initial capacity – at least enough for n+1. */
        cache_cap = (n > 16) ? (idx_t)(n + 1) : 16;
        a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!a_cache) exit(1);              /* allocation failed */

        /* Initialise the first six terms explicitly. */
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)1;
        a_cache[3] = (seq_t)8;
        a_cache[4] = (seq_t)5;
        a_cache[5] = (seq_t)11;
        cache_len = 6;
    } else if (cache_cap <= n) {
        /* Grow the array – double until it fits. */
        while (cache_cap <= n) cache_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, cache_cap * sizeof(seq_t));
        if (!a_cache) exit(1);
    }

    /* Compute any new terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 6) {
            /* Already initialised; nothing to do. */
            continue;
        }
        a_cache[i] = ((seq_t)7 * a_cache[i-3]) + ((seq_t)8 * a_cache[i-6]);
    }
    cache_len = n + 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
