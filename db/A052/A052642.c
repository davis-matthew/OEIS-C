/*
 * A052642  Expansion of e.g.f. x^2*(2+x-x^2)/(1-x).
 * https://oeis.org/A052642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n=0,1; a(2)=4; a(3)=18; a(n) = 2 * n! for n >= 4
 *            (n! is the ordinary factorial)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for factorials */
    static seq_t *fact = NULL;      /* factorial array */
    static idx_t fact_cap = 0;      /* allocated capacity */
    static idx_t fact_last = 0;     /* highest computed factorial index */

    /* Ensure factorial cache is initialized up to n */
    if (n >= 2) {
        if (fact == NULL) {
            /* Allocate for at least indices 0 and 1 */
            fact_cap = 2;
            fact_last = 1;
            fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
            if (!fact) return (seq_t)0; /* allocation failure, return 0 as safe default */
            fact[0] = (seq_t)1;
            fact[1] = (seq_t)1;
        }
        if (fact_last < n) {
            /* Increase capacity if needed */
            if (n >= fact_cap) {
                size_t new_cap = n + 1;
                seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* realloc failure */
                fact = tmp;
                fact_cap = new_cap;
            }
            /* Compute missing factorials */
            for (idx_t i = fact_last + 1; i <= n; ++i) {
                fact[i] = fact[i - 1] * (seq_t)i;
            }
            fact_last = n;
        }
    } else {
        /* For n=0 or 1, factorials are not needed */
        if (fact == NULL) {
            fact_cap = 2;
            fact_last = 1;
            fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
            if (!fact) return (seq_t)0;
            fact[0] = (seq_t)1;
            fact[1] = (seq_t)1;
        }
    }

    /* Compute a(n) according to the explicit rules */
    if (n == 0 || n == 1) {
        return (seq_t)0;
    } else if (n == 2) {
        return (seq_t)4;
    } else if (n == 3) {
        return (seq_t)18;
    } else {
        /* n >= 4 */
        seq_t res = fact[n] * (seq_t)2;
        return res;
    }
}
