/*
 * A143643  Numerators of the lower principal convergents and the lower
 *          intermediate convergents to 3^(1/2).
 * https://oeis.org/A143643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 4*a(n-2)-a(n-4); a(1)=1, a(2)=3, a(3)=5, a(4)=12
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc, abort */
#include <stddef.h> /* size_t */

/* Return the n-th term of sequence A143643 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined but harmless */

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated elements (index 0..cap-1) */
    static idx_t  last  = 0;   /* last computed index */

    /* Ensure we have room for indices up to n (including 4 for the recurrence) */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 5)               /* at least indices 1..4 */
            new_cap = 5;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)
            abort();                    /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first four terms once (if not already done) */
    if (last == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)12;
        last    = 4;
    }

    /* If n is within the already computed range, return immediately */
    if (n <= last)
        return cache[n];

    /* Compute terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i - 2] - cache[i - 4];
    }

    last = n;
    return cache[n];
}
