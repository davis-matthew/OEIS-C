/*
 * A077613  Number of adjacent pairs of form (even,odd) among all permutations
 *          of {1,2,...,n}. Also, number of adjacent pairs of form (odd,even).
 * https://oeis.org/A077613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2)*ceil(n/2)*(n-1)! (OEIS A077613)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)   /* the sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    /* Cache for factorials: fact[k] = k!  for 0 ≤ k ≤ fact_max */
    static seq_t *fact = NULL;
    static idx_t fact_max = 0; /* highest k whose factorial is stored */

    idx_t k = n - 1; /* we need (n-1)! */

    /* Ensure the cache exists and has entry for 0! */
    if (fact == NULL) {
        fact = (seq_t *)malloc(sizeof(seq_t));
        if (!fact) return (seq_t)0;          /* allocation failed */
        fact[0] = (seq_t)1;                 /* 0! = 1 */
        fact_max = 0;
    }

    /* Extend the cache if necessary */
    if (k > fact_max) {
        size_t new_cap = (size_t)(k + 1);    /* need indices 0 .. k */
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        fact = tmp;

        /* Compute missing factorials */
        for (idx_t i = fact_max + 1; i <= k; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_max = k;
    }

    /* floor(n/2) and ceil(n/2) cast to seq_t */
    seq_t e = (seq_t)(n >> 1);                 /* floor(n/2) */
    seq_t o = (seq_t)((n + 1) >> 1);           /* ceil(n/2) */

    return e * o * fact[k];                    /* a(n) = e*o*(n-1)! */
}
