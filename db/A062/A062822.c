/*
 * A062822  Sum of divisors of the squarefree numbers: sigma(A005117(n)).
 * https://oeis.org/A062822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ( A005117(n) ) = σ( the n-th squarefree number )
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sigma_of_squarefree.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used but safe */

    /* Cache for already computed terms */
    static seq_t cache[10001];     /* enough for n <= 10000 */
    static size_t cached = 0;      /* number of terms cached (>=1) */
    static idx_t next_candidate = 1;  /* candidate to test for squarefreeness */

    /* Extend cache up to requested n */
    while (cached < n) {
        /* Find next squarefree integer */
        while (1) {
            idx_t cand = next_candidate;
            ++next_candidate;
            /* Check if cand has any squared prime factor */
            idx_t temp = cand;
            idx_t p = 2;
            int is_sqfree = 1;
            while (p * p <= temp) {
                if (temp % (p * p) == 0) { is_sqfree = 0; break; }
                ++p;
            }
            if (is_sqfree) {
                cache[cached] = sigma_of_squarefree(cand);
                ++cached;
                break;
            }
        }
    }

    return cache[n - 1];
}
