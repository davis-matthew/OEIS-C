/*
 * A152437  (17^n - 1)/(2^(5 - (n % 2))).
 * https://oeis.org/A152437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A152437
    Formula:  a(n) = (17^n – 1) / (2^(5 – (n mod 2)))
    Recurrence:  a(0)=0, a(1)=1, a(2)=9, a(3)=307,
                 a(n) = 290*a(n-2) – 289*a(n-4)   for n ≥ 4
**************************************************************************/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t  cap   = 0;        /* current allocated capacity   */
    static size_t  len   = 0;        /* highest index computed + 1   */

    /* Ensure capacity for indices up to n */
    if (cap <= (size_t)n) {
        size_t new_cap = (cap == 0 ? 4 : cap);
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                       /* grow exponentially   */
        /* cast needed for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise first four terms only once */
    if (len == 0) {
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 9;
        cache[3] = 307;
        len = 4;
    }

    /* Return if the requested term is already known */
    if (n < len)
        return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* i >= 4 because len is at least 4 at this point */
        cache[i] = 290 * cache[i - 2] - 289 * cache[i - 4];
    }
    len = (size_t)n + 1;
    return cache[n];
}
