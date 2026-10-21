/*
 * A210293  Half the number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          having at most one duplicate clockwise edge difference.
 * https://oeis.org/A210293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A210293: a(n) satisfies the recurrence
        a(n) = 11*a(n-1) + 42*a(n-2) + 20*a(n-3) – 18*a(n-4)
    for n ≥ 5 with initial values
        a(1) = 112,  a(2) = 1578,  a(3) = 22212,  a(4) = 312704.    */
#include <stdlib.h>
#include <stdint.h>

/* Function to compute the n-th term (n≥1).  That is,
   a(1) = 112, a(2) = 1578, …                                         */
seq_t a(idx_t n)
{
    /* Basic sanity check: the sequence is defined only for n≥1.  */
    if (n < 1) return (seq_t)0;      /* undefined, return 0 */

    static seq_t *cache = NULL;      /* array of previously computed terms   */
    static size_t cap = 0;           /* allocated capacity                   */
    static idx_t max_computed = 0;   /* highest n for which cache holds a value */

    /* If we already have a value for n, just return it.   */
    if (n <= max_computed) {
        return cache[n - 1];
    }

    /* Allocate / grow the cache to be able to store up to n values.  */
    if (cache == NULL) {
        cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    }
    while (cap < n) {
        size_t new_cap = cap * 2;
        if (new_cap == 0) new_cap = cap + 64;   /* fallback for very small cap */
        new_cap = (new_cap < n) ? n : new_cap;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cap = new_cap;
    }

    /* Compute all missing terms up to n using the recurrence.  */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i - 1] = (seq_t)112;
        } else if (i == 2) {
            cache[i - 1] = (seq_t)1578;
        } else if (i == 3) {
            cache[i - 1] = (seq_t)22212;
        } else if (i == 4) {
            cache[i - 1] = (seq_t)312704;
        } else {
            seq_t t1 = ((seq_t)11) * cache[i - 2];
            seq_t t2 = ((seq_t)42) * cache[i - 3];
            seq_t t3 = ((seq_t)20) * cache[i - 4];
            seq_t t4 = ((seq_t)18) * cache[i - 5];
            cache[i - 1] = t1 + t2 + t3 - t4;
        }
    }

    max_computed = n;
    return cache[n - 1];
}
