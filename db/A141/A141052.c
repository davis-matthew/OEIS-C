/*
 * A141052  Number of runs or rising sequences of length 2 among all
 *          permutations of n.
 * https://oeis.org/A141052
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*a(n-1) + (n-1)!/6, a(2)=1, a(3)=4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;     /* size of the allocated cache array */
static idx_t max_computed = 1;   /* the largest n already computed (>=1) */
static seq_t fact = 0;           /* factorial of max_computed (fact = max_computed!) */

/* Compute the n-th term of OEIS A141052 */
seq_t a(idx_t n)
{
    if (n < 2)          /* below the offset – undefined behaviour is acceptable */
        return 0;

    /* Grow the cache array if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)               /* allocation failed – return 0 as a safe value */
            return 0;
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* If the requested index is already cached, return it straight away */
    if (n <= max_computed)
        return cache[n];

    /* Initialise the cache the first time it is used */
    if (max_computed < 2) {
        cache[2] = (seq_t)1;     /* a(2) = 1 */
        max_computed = 2;
        fact = (seq_t)2;         /* fact = 2! = 2  ->  (max_computed)!  */
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* term = (i-1)! / 6  (exact integer division) */
        seq_t term = fact / (seq_t)6;
        /* a(i) = (i+1) * a(i-1) + term */
        seq_t ai = ((seq_t)(i + 1)) * cache[i - 1] + term;
        cache[i] = ai;
        /* Update factorial: now fact holds i!  */
        fact *= (seq_t)i;
    }
    max_computed = n;
    return cache[n];
}
