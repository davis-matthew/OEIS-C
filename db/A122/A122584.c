/*
 * A122584  Expansion of x*(1+x)*(1-2*x)/(1 - 2*x - x^2 + 2*x^3 - x^4).
 * https://oeis.org/A122584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 33, u64 n <= 62, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A122584:
   Recurrence: a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) + a(n-4)
   Initial conditions: a(1) = a(2) = a(3) = a(4) = 1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t last_computed = 0;

    /* Ensure sufficient allocated space. */
    if (n > capacity) {
        idx_t new_cap;
        if (capacity == 0)
            new_cap = (n < 4 ? 4 : n);
        else
            new_cap = (n > capacity * 2 ? n : capacity * 2);
        if (new_cap < 4) new_cap = 4;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)               /* allocation failed – return zero */
            return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the first four terms if this is the first call. */
    if (last_computed == 0) {
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
        last_computed = 4;
    }

    /* If the value has already been computed, just return it. */
    if (n <= last_computed)
        return cache[n - 1];

    /* Compute terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 2];   /* a(i-1) */
        seq_t a2 = cache[i - 3];   /* a(i-2) */
        seq_t a3 = cache[i - 4];   /* a(i-3) */
        seq_t a4 = cache[i - 5];   /* a(i-4) */

        cache[i - 1] = 2 * a1 + a2 - 2 * a3 + a4;
    }
    last_computed = n;
    return cache[n - 1];
}
