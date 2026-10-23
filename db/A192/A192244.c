/*
 * A192244  0-sequence of reduction of triangular number sequence by x^2 ->
 *          x+1.
 * https://oeis.org/A192244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1, a(3) = 7, a(4) = 17, a(5) = 47, a(6) = 110,
 *            a(7) = 250 Recurrence (for n > 7): a(n) = 4*a(n-1) - 3*a(n-2) -
 *            5*a(n-3) + 5*a(n-4) + 3*a(n-5) - 2*a(n-6) - a(n-7)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based array of computed terms */
static idx_t cache_cap = 0;          /* allocated size (terms + 1)  */
static idx_t cache_len = 0;          /* highest n already computed */

/* Base terms a(1) .. a(7) */
static const seq_t init[8] = {0, 1, 1, 7, 17, 47, 110, 250};

seq_t a(idx_t n)
{
    /* Fast return for the first seven values */
    if (n <= 7) return init[n];

    /* Allocate or enlarge cache if necessary */
    if (cache == NULL || cache_cap < n + 1) {
        idx_t new_cap = (cache_cap < 8 ? 8 : cache_cap);
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) /* Allocation failed – program will exit */
            abort();
        cache = tmp;
        cache_cap = new_cap;

        /* Initialise missing base terms if cache just started */
        if (cache_len == 0) {
            for (idx_t i = 1; i <= 7; ++i) cache[i] = init[i];
            cache_len = 7;
        }
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Positive contributions */
        seq_t sum_pos = cache[i-1] * ((seq_t)4)
                      + cache[i-4] * ((seq_t)5)
                      + cache[i-5] * ((seq_t)3);

        /* Negative contributions */
        seq_t sum_neg = cache[i-2] * ((seq_t)3)
                      + cache[i-3] * ((seq_t)5)
                      + cache[i-6] * ((seq_t)2)
                      + cache[i-7];

        cache[i] = sum_pos - sum_neg;
    }
    cache_len = n;
    return cache[n];
}
