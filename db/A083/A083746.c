/*
 * A083746  a(1) = 1, a(2) = 2; for n>2, a(n) = 3*(n-2)*(n-2)!.
 * https://oeis.org/A083746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(n-2)*(n-2)! for n > 2 Initial values: a(1) = 1, a(2) =
 *            2, a(3) = 3. Uses incremental recurrence: a(n) = a(n-1) *
 *            (n-2)^2 / (n-3) for n > 3 (Division is exact; we first divide to
 *            avoid overflow.)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc / realloc */

seq_t a(idx_t n)
{
    /* Undefined for n < 1 */
    if (n < 1) return (seq_t)0;

    /* Static cache */
    static seq_t *cache      = NULL;
    static idx_t  capacity   = 0;
    static idx_t  last_used  = 0;   /* highest n computed so far */

    /* Grow cache if needed */
    if (n >= capacity) {
        size_t new_cap = (capacity == 0) ? 8 : (size_t)capacity * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        /* initialise the new part to 0 */
        for (idx_t i = capacity; i < new_cap; ++i) cache[i] = 0;
        capacity = (idx_t)new_cap;
    }

    /* Fill missing entries up to n */
    if (last_used < n) {
        for (idx_t i = last_used + 1; i <= n; ++i) {
            if (i == 1) cache[i] = (seq_t)1;
            else if (i == 2) cache[i] = (seq_t)2;
            else if (i == 3) cache[i] = (seq_t)3;
            else {
                /* a(i) = a(i-1) * (i-2)^2 / (i-3)   (division exact) */
                seq_t t = cache[i-1] / ((seq_t)(i-3));
                seq_t mult = ((seq_t)(i-2)) * ((seq_t)(i-2));
                cache[i] = t * mult;
            }
        }
        last_used = n;
    }

    return cache[n];
}
