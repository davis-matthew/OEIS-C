/*
 * A081195  a(n) = 10*a(n-1)-24*a(n-2) for n>1, a(0)=1, a(1)=25.
 * https://oeis.org/A081195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 24*a(n-2) (a(0)=1, a(1)=25)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n‑th term of OEIS A081195.  The sequence satisfies
 * the recurrence a(0)=1, a(1)=25 and a(n)=10*a(n-1)-24*a(n-2) for n>1.
 * We cache computed values so that calls in any order remain fast.
 */
seq_t a(idx_t n)
{
    /* Static cache: computed_size = number of terms already computed
     * (indices 0 .. computed_size-1).  capacity is the allocated size.
     */
    static seq_t *cache      = NULL;
    static idx_t computed_size = 0;
    static idx_t capacity     = 0;

    /* Ensure the internal array has space for terms up to index n. */
    if (capacity < n + 1) {
        idx_t new_cap = (capacity == 0) ? 4 : capacity * 2;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* memory allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* If this is the first call, initialise the cache with the two
     * known start values.  We only initialise values that have not
     * yet been set.
     */
    if (computed_size == 0) {
        cache[0] = (seq_t)1;
        computed_size = 1;
    }
    if (computed_size == 1) {
        cache[1] = (seq_t)25;
        computed_size = 2;
    }

    /* Compute any new terms up to index n, using the recurrence. */
    for (idx_t i = computed_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)25;
        } else {
            cache[i] = cache[i-1] * ((seq_t)10) - cache[i-2] * ((seq_t)24);
        }
    }
    computed_size = n + 1;

    return cache[n];
}
