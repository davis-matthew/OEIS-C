/*
 * A152885  Number of descents beginning and ending with an odd number in all
 *          permutations of {1,2,...,n}.
 * https://oeis.org/A152885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A152885
   Formula:
        a(2n)   = (2n-1)! * C(n,   2)
        a(2n+1) = (2n)!   * C(n+1, 2)
   (n ≥ 1, with a(1) = a(2) = 0)
*/

#include <stdlib.h>           /* malloc, realloc */

/* Cache for factorials: fact_cache[i] = i! */
static seq_t *fact_cache = NULL;
static idx_t  fact_cache_size = 0;   /* largest i computed */

/* Ensure factorials up to n are stored in fact_cache. */
static void ensure_fact_cache(idx_t n)
{
    if (fact_cache == NULL) {                     /* first allocation */
        fact_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!fact_cache) return;
        fact_cache[0] = (seq_t)1;
        fact_cache_size = 0;
    } else if (fact_cache_size >= n) {            /* already have up to n */
        return;
    } else {                                     /* grow the cache */
        seq_t *tmp = (seq_t *)realloc(fact_cache,
                                      (n + 1) * sizeof(seq_t));
        if (!tmp) return;
        fact_cache = tmp;
    }

    for (idx_t i = fact_cache_size + 1; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_cache_size = n;
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    if (n < 3)                    /* a(1) = a(2) = 0 */
        return (seq_t)0;

    /* n >= 3 */
    idx_t k = n / 2;              /* floor(n/2) */

    /* Compute factorial part: (n-1)! */
    ensure_fact_cache(n - 1);
    seq_t fact = fact_cache[n - 1];

    /* Compute the binomial coefficient factor */
    seq_t comb;
    if ((n & 1) == 0) {          /* n = 2k */
        comb = ((seq_t)k * (k - 1)) / 2;      /* C(k,2) */
    } else {                     /* n = 2k+1 */
        comb = ((seq_t)(k + 1) * k) / 2;      /* C(k+1,2) */
    }

    return fact * comb;
}
