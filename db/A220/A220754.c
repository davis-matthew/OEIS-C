/*
 * A220754  Number of ordered triples (a,b,c) of elements of the symmetric
 *          group S_n such that the triple a,b,c generates a transitive group.
 * https://oeis.org/A220754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = (n!)^3 - Σ_{k=1}^{n-1} a(k) * ( (n-1)! / (k-1)!
 *            ) * ((n-k)!)^2 (derived from the recurrence given in OEIS
 *            A220754)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* The function is required to have external linkage */
seq_t a(idx_t n) {
    /* static cache for factorials and a(n) values */
    static seq_t *fact = NULL;     /* factorials: fact[i] = i!          */
    static seq_t *an   = NULL;     /* a(n) values: an[i] = a(i)        */
    static idx_t   max_n = 0;      /* maximum n currently cached        */
    static size_t  cache_cap = 0;  /* current allocated size           */

    /* resize cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (n + 1) * 2;
        seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        seq_t *new_an   = (seq_t *)realloc(an,   new_cap * sizeof(seq_t));
        if (!new_fact || !new_an) return 0; /* allocation failed */
        /* initialise new entries if we are extending beyond previous limit */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            new_fact[i] = 0;
            new_an[i]   = 0;
        }
        fact = new_fact;
        an   = new_an;
        cache_cap = new_cap;
    }

    /* base case  n >= 1   */
    if (n == 0) return 0;

    /* initialise factorial[0] = 1 if not already */
    if (max_n == 0) {
        fact[0] = 1;
        an[0]   = 0;    /* unused */
    }

    /* compute factorials incrementally */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (idx_t)i;
    }

    /* compute a(i) values incrementally */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1) {
            an[1] = 1;
            continue;
        }

        /* (i!)^3 */
        seq_t fi  = fact[i];
        seq_t ans = fi * fi;      /* (i!)^2 */
        ans *= fi;                /* (i!)^3 */

        /* sum over k=1..i-1 */
        seq_t sum = 0;
        seq_t ratio = fact[i - 1];   /* (i-1)!  / fact[k-1]   */
        for (idx_t k = 1; k < i; ++k) {
            seq_t term = an[k];
            term *= ratio;                                   /* a(k) * P(k,i)   */
            seq_t fsq = fact[i - k];
            fsq *= fsq;                                        /* ((i-k)!)^2      */
            term *= fsq;                                       /* complete term   */
            sum += term;                                       /* accumulate sum  */

            if (k < i - 1) {
                /* prepare ratio for next k: divide by k   */
                ratio /= (idx_t)k;
            }
        }

        ans -= sum;              /* a(i) = (i!)^3 - sum */
        an[i] = ans;
    }

    max_n = n;
    return an[n];
}
