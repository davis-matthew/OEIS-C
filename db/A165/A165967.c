/*
 * A165967  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for n ≥ 1: a(n) = 23 * (a(n-1)+a(n-2)+…+a(n-9)) - 276 *
 *            a(n-10) + rhs(n) where rhs(n) = 2 for 1 ≤ n ≤ 9, 1 for n = 10, 0
 *            otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of already computed a(i) */
static idx_t cache_size = 0;    /* number of elements stored in cache */

/* Ensure that cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_size = n + 1;
    seq_t *new_arr = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_arr) exit(1);               /* memory failure */

    /* copy old entries into new array */
    for (idx_t i = 0; i < cache_size; ++i) new_arr[i] = cache[i];
    /* free the old array */
    free(cache);
    /* replace with the new, larger array */
    cache = new_arr;

    /* compute all missing terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* base case */
        if (i == 0) {
            cache[0] = 1;
            continue;
        }

        /* sum of previous up to 9 terms */
        seq_t sum = 0;
        idx_t kmax = i < 9 ? i : 9;
        for (idx_t k = 1; k <= kmax; ++k)
            sum += cache[i - k];

        /* recurrence part */
        seq_t term = sum * (seq_t)23;
        if (i >= 10) term -= cache[i - 10] * (seq_t)276;

        /* right-hand side constant depending on i */
        seq_t rhs;
        if (i <= 9)       rhs = 2;
        else if (i == 10) rhs = 1;
        else              rhs = 0;

        cache[i] = term + rhs;
    }

    cache_size = new_size;
}

/* External linkage: sequence accessor */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
