/*
 * A015448  a(0) = 1, a(1) = 1, and a(n) = 4*a(n-1) + a(n-2) for n >= 2.
 * https://oeis.org/A015448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(n) = 4*a(n-1) + a(n-2) for n >= 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic array of computed terms */
    static idx_t   len   = 0;     /* number of terms stored (cache[0..len-1]) */
    static size_t  cap   = 0;     /* current allocation capacity */

    /* Initialise the table with the first two terms if it is empty */
    if (len == 0) {
        if (cap < 2) {
            size_t new_cap = 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* memory failure */
            cache = tmp;
            cap   = new_cap;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        len = 2;
    }

    /* If the requested index is already cached, return it */
    if (n < len) return cache[n];

    /* Ensure the array is large enough to hold up to index n */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* memory failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute all missing terms up to n using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            cache[i] = cache[i-1] * ((seq_t)4) + cache[i-2];
        }
    }

    len = n + 1;   /* we now have terms 0..n stored */
    return cache[n];
}
