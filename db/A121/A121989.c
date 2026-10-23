/*
 * A121989  a(n) = (n - 1)*(a(n - 2) - a(n - 3) + 1), a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A121989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n - 1)*(a(n - 2) - a(n - 3) + 1), a(0) = a(1) = a(2) = 1
 * Range:     i8 n <= 8, i16 n <= 13, i32 n <= 20, i64 n <= 33, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache    = NULL;
static bool  *computed = NULL;
static idx_t  cap      = 0;

/* Ensure the cache can hold values up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cap > n) return;

    idx_t new_cap = cap ? cap : 8;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) abort();

    bool *tmp2 = (bool *)realloc(computed, new_cap * sizeof(bool));
    if (!tmp2 && new_cap > 0) abort();

    cache    = tmp;
    computed = tmp2;

    for (idx_t i = cap; i < new_cap; ++i) computed[i] = false;
    cap = new_cap;
}

/* Main function required by the exercise. */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* Initialize base terms once. */
    if (!computed[0]) {
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        computed[0] = computed[1] = computed[2] = true;
    }

    if (n < 3) return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t i = 3; i <= n; ++i) {
        if (computed[i]) continue;
        cache[i] = (seq_t)(i - 1) * (cache[i - 2] - cache[i - 3] + 1);
        computed[i] = true;
    }

    return cache[n];
}
