/*
 * A090015  Permanent of (0,1)-matrix of size n X (n+d) with d=5 and n-1 zeros
 *          not on a line.
 * https://oeis.org/A090015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)*a(n-1) + (n-2)*a(n-2), a(1)=6, a(2)=36
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
 * Returns the n-th term (n >= 1) of OEIS sequence A090015.
 *
 * The function uses a simple linear recurrence:
 *     a(n) = (n+4)*a(n-1) + (n-2)*a(n-2)
 *
 * It keeps a static cache so that repeated calls are O(1) after
 * the first computation up to the maximum n requested so far.
 */
seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity (in elements) */
    static idx_t  last  = 0;   /* largest n that has been computed */

    if (n < 1)    /* undefined behaviour, but return zero to be safe */
        return (seq_t)0;

    /* Ensure the cache is large enough */
    if (cap < (size_t)n) {
        size_t newcap = cap ? cap << 1U : 2;
        while (newcap < (size_t)n) newcap <<= 1U;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!cache)   /* memory allocation failed – abort with zero */
            return (seq_t)0;
        cap = newcap;
    }

    /* Initialize the first two terms if they are not yet stored */
    if (last < 1) {
        cache[0] = (seq_t)6;
        last = 1;
    }
    if (last < 2) {
        cache[1] = (seq_t)36;
        last = 2;
    }

    /* If the requested n has already been computed, return it */
    if (n <= last)
        return cache[n - 1];

    /* Compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 2];   /* a(i-1) */
        seq_t a_nm2 = cache[i - 3];   /* a(i-2) */
        cache[i - 1] = ((seq_t)(i + 4)) * a_nm1 + ((seq_t)(i - 2)) * a_nm2;
    }
    last = n;                            /* update highest computed value */
    return cache[n - 1];
}
