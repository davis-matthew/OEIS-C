/*
 * A070968  Number of cycles in the complete bipartite graph K(n,n).
 * https://oeis.org/A070968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} C(n,k)^2 * k! * (k-1)! / 2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0; /* offset is 1 */

    /* --- factorial cache (pre-compute up to n) --- */
    static seq_t *fact = (seq_t*)NULL; /* fact[0] = 1, fact[i] = i! */
    static size_t fact_n = 0;         /* current capacity of fact array */

    /* First allocation */
    if (fact_n == 0) {
        fact = (seq_t*)malloc(sizeof(seq_t));
        if (!fact) return 0;
        fact[0] = (seq_t)1;
        fact_n = 1;
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > fact_n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(fact, sizeof(seq_t) * new_size);
        if (!tmp) return 0;          /* allocation failed */
        fact = tmp;
        for (size_t i = fact_n; i < new_size; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_n = new_size;
    }

    /* Sum the terms */
    seq_t sum = (seq_t)0;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t c    = binom(n, k);                /* C(n,k) */
        seq_t term = c * c;                      /* square it */
        term = term * fact[(size_t)k];           /* * k!      */
        term = term * fact[(size_t)(k - 1)];     /* * (k-1)!  */
        term = term / (seq_t)2;                  /* divide by 2 */
        sum += term;
    }

    return sum;
}
