/*
 * A160650  a(n) = A001222(A160649(n)) = A160649(n+1) - A160649(n); where
 *          A001222(m) is the sum of prime-factorization exponents of m (or,
 *          A001222(m) is the number of primes dividing m, counted with
 *          multiplicity).
 * https://oeis.org/A160650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = A001222(A160649(n)) = A160649(n+1) - A160649(n);
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 2, ...  (OEIS A160650) */

#include <stdlib.h>
#include "../utils/count_prime_factors.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache of a[1..max_comp] */
    static seq_t *a_vals  = NULL;
    static idx_t  cache_cap = 0;  /* current allocated capacity (1‑based) */
    static idx_t  max_comp  = 0;  /* maximum index that has been computed */
    static seq_t  curA      = 0;  /* A_n for n = max_comp */

    /* Initialize cache with the first term */
    if (max_comp == 0) {
        cache_cap = 2;                           /* allocate room for a[1] */
        a_vals = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        curA   = (seq_t)2;                       /* A_1 = 2 */
        a_vals[1] = (seq_t)1;                    /* a_1 = Ω(2) = 1 */
        max_comp = 1;
    }

    /* Extend cache if needed for index n */
    if (n > max_comp) {
        /* Grow capacity to accommodate n+1 (1‑based array) */
        if (n + 1 > cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
            while (new_cap <= n) new_cap *= 2;
            a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Compute a[n] iteratively from the last cached value */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            /* A_i = A_{i-1} + a_{i-1} */
            seq_t nextA = curA + a_vals[i - 1];
            /* a_i = Ω(A_i) */
            seq_t ai = count_prime_factors(nextA);
            /* Update for next iteration */
            curA = nextA;
            a_vals[i] = ai;
        }
        max_comp = n;
    }

    return a_vals[n];
}
