/*
 * A208557  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 17*a(n-2) + 6*a(n-3) - 36*a(n-4) * with initial
 *            values a(1)=12, a(2)=144, a(3)=420, a(4)=2508
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)          /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* static cache for memoisation – works for any order of calls   */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t  last   = 0;   /* largest index with a known value     */

    /* we need array indices 0..n ; index 0 is unused.  Ensure capacity */
    size_t required = (size_t)n + 1;
    if (required < 5)          /* we must store at least a(1)..a(4)   */
        required = 5;
    if (cap < required) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp && required > 0) {
            abort();           /* allocation failed */
        }
        cache = tmp;
        cap   = required;
    }

    /* initialise the first four terms only once */
    if (last == 0) {
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)144;
        cache[3] = (seq_t)420;
        cache[4] = (seq_t)2508;
        last = 4;
    }

    if (n <= last)           /* already computed */
        return cache[n];

    /* compute terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = cache[i-1]
                   + ((seq_t)17 * cache[i-2])
                   + ((seq_t)6  * cache[i-3])
                   - ((seq_t)36 * cache[i-4]);
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
