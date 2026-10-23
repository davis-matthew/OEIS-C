/*
 * A120743  a(n) = (1/2)*(1 + 3*i)^n + (1/2)*(1 - 3*i)^n where i = sqrt(-1).
 * https://oeis.org/A120743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 37, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120743
 *
 * a(n) = (1/2)*(1+3*i)^n + (1/2)*(1-3*i)^n
 *      = 2*a(n-1) - 10*a(n-2),   a(1)=1, a(2)=-8
 *
 * The function below computes a(n) for any n >= 1 using the
 * linear recurrence.  Results are cached so that repeated calls
 * are O(1) after the initial computation.  The implementation
 * is fully generic for the `seq_t` and `idx_t` types supplied
 * by the build system.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* indices 0..max_cached – element 0 is unused (offset is 1) */
    static seq_t *cache     = NULL;
    static idx_t cap        = 0;
    static idx_t max_cached = 0;

    /* sequence is undefined for n < 1; return 0 (behaviour below offset is irrelevant) */
    if (n == 0) return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 3;                         /* indices 0,1,2 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)0;             /* unused */
        cache[1] = (seq_t)1;             /* a(1) */
        cache[2] = (seq_t)(-8);          /* a(2) */
        max_cached = 2;
    }

    /* extend cache if needed */
    if (n > max_cached) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;          /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;

        for (idx_t i = max_cached + 1; i <= n; ++i) {
            cache[i] = (seq_t)( 2 * cache[i-1] - 10 * cache[i-2] );
        }
        max_cached = n;
    }

    return cache[n];
}
