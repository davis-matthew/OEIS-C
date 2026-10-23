/*
 * A162270  a(n) = ((5+sqrt(2))*(3+sqrt(2))^n + (5-sqrt(2))*(3-sqrt(2))^n)/2.
 * https://oeis.org/A162270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((5+sqrt(2))*(3+sqrt(2))^n + (5-sqrt(2))*(3-sqrt(2))^n)/2
 *            Recurrence: a(0) = 5, a(1) = 17, a(n) = 6*a(n-1) - 7*a(n-2) (n >
 *            1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* Cached values of the sequence   */
static idx_t  cache_len  = 0;      /* Number of terms currently cached */
static size_t cache_cap  = 0;      /* Allocated array capacity        */

/* Ensure that terms up to index n are computed and stored in cache[]. */
static void ensure_upto(idx_t n)
{
    /* Already have enough terms? */
    if (cache_len > n)
        return;

    /* Determine how many slots are needed (at least 2 for the base case). */
    size_t need = (size_t)(n + 1);
    if (need < 2) need = 2;

    /* Grow the array if required. */
    if (cache_cap < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(1);          /* memory failure */
        cache = tmp;
        cache_cap = need;
    }

    /* First time: initialise the first two terms. */
    if (cache_len == 0) {
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)17;
        cache_len = 2;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = ((seq_t)6) * cache[i - 1] - ((seq_t)7) * cache[i - 2];
    }

    /* Update the count of cached terms. */
    if (cache_len < n + 1)
        cache_len = n + 1;
}

/* Return the nth term of A162270. */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return cache[n];
}
