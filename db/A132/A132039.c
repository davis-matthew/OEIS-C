/*
 * A132039  E.g.f.: A(x) = Sum_{n>=0} a(n)*x^n/n! = exp( Sum_{n>=0}
 *          a(n)*x^(n+1)/(n+1) ) with a(0) = 1.
 * https://oeis.org/A132039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(0) = 1; a(n+1) = Sum_{k=0}^{n} (n! / k!) * a(k) * a(n-k) for n
 *            >= 0 (nonlinear recurrence from OEIS A132039)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *a_seq = NULL;
    static seq_t *fact  = NULL;
    static size_t a_cap  = 0;
    static size_t fact_cap = 0;
    static idx_t max_comp = 0;   /* highest index already computed */

    /* initial allocation for index 0 */
    if (a_seq == NULL) {
        a_cap = 1;
        a_seq = (seq_t*)malloc(a_cap * sizeof(seq_t));
        a_seq[0] = (seq_t)1;
    }
    if (fact == NULL) {
        fact_cap = 1;
        fact = (seq_t*)malloc(fact_cap * sizeof(seq_t));
        fact[0] = (seq_t)1;
    }

    /* grow arrays if necessary */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = (size_t)n + 1;
        a_seq = (seq_t*)realloc(a_seq, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }
    if ((size_t)n + 1 > fact_cap) {
        size_t new_cap = (size_t)n + 1;
        fact = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
        fact_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* factorial i */
        if (i > 0) {
            fact[i] = fact[i-1] * (seq_t)i;
        } else {
            fact[i] = (seq_t)1;
        }

        /* recurrence for a[i] (i >= 1) */
        if (i == 0) {
            a_seq[0] = (seq_t)1;
            continue;
        }
        idx_t m = i - 1;
        seq_t factorial_m = fact[m];
        seq_t sum = (seq_t)0;
        seq_t f = factorial_m;                /* f = m! / k!  (k=0 initially) */
        for (idx_t k = 0; k <= m; ++k) {
            seq_t prod = a_seq[k];
            prod *= a_seq[m - k];
            prod *= f;
            sum += prod;
            f /= (seq_t)(k + 1);              /* update f for next k */
        }
        a_seq[i] = sum;
    }
    max_comp = n;
    return a_seq[n];
}
