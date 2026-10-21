/*
 * A083076  Third row of number array A083075.
 * https://oeis.org/A083076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*7^n + 1) / 3. Recurrence: a(0) = 1, a(n) = 7*a(n-1) -
 *            2 (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached = 0;  /* index of last computed value */
    static size_t cap = 0;    /* allocated capacity */

    /* Ensure capacity for indices 0 … n. */
    if (n >= cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialize a(0) if this is the first call. */
    if (cached == 0 && cap > 0) {
        cache[0] = (seq_t)1;
        cached = 0;
    }

    /* Compute any missing values up to n using the recurrence. */
    for (idx_t k = cached + 1; k <= n; ++k) {
        seq_t val = (seq_t)7 * cache[k - 1];
        val -= (seq_t)2;
        cache[k] = val;
    }

    cached = n;
    return cache[n];
}
